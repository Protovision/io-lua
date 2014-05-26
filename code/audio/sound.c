#include "audio.h"

#define	MAX_SOUNDS	128
#define	MAX_SOUNDS_HASH 64	
#define	MAX_CHANNELS	8	

#define CHANNEL_LOOP	1
#define CHANNEL_PAUSED	2

sound_t	sounds[MAX_SOUNDS];
sound_t	*sounds_hashtable[MAX_SOUNDS_HASH];

channel_t	channels[MAX_CHANNELS];
channel_t	*freechannels;

extern SDL_AudioSpec	audio_spec;
extern void		audio_lock();
extern void		audio_unlock();

static void	channel_free(channel_t *c)
{
	c->sound = NULL;
	c->next_free = freechannels;
	freechannels = c;
}

static channel_t	*channel_alloc()
{
	channel_t *c;

	if (freechannels == NULL)
		return NULL;
	c = freechannels;
	freechannels = c->next_free;
	c->next_free = NULL;
	return c;
}

void	channel_clear()
{
	channel_t *p, *q;

	memset(channels, 0, sizeof(channels));
	p = channels;
	q = p + MAX_CHANNELS;
	while (--q > p) {
		q->next_free = q-1;
	}
	q->next_free = NULL;
	freechannels = p + MAX_CHANNELS - 1;
}

void	sound_init()
{
	channel_clear();
	memset(sounds, 0, sizeof(sounds));
}


void	sound_shutdown() {}

static unsigned int	sound_hash(const char *file)
{
	int	i;
	unsigned int hash;
	char	ch;

	hash = 0;
	i = 0;
	while (*file) {
		ch = tolower(*file);
		if (ch == '.') break;
		if (ch == '\\') ch = '/';
		hash += (int)(ch)*(i+119);
		++i;
		++file;
	}
	hash &= (MAX_SOUNDS_HASH-1);
	return hash;
}

static sound_t	*sound_alloc(const char *file)
{
	int i;
	unsigned int hash;
	sound_t	*s;

	hash = sound_hash(file);

	for (s = sounds_hashtable[hash]; s != NULL; s = s->next) {
		if (strcmp(s->path, file) == 0) {
			return s;
		}
	}

	for (i = 0; i != MAX_SOUNDS; ++i) {
		if (sounds[i].path[0] == 0) break;
	}
	
	if (i == MAX_SOUNDS) {
		FATAL("Maximum number of loaded sounds exceeded");
	}

	s = &sounds[i];
	memset(s, 0, sizeof(sound_t));
	strcpy(s->path, file);
	s->next = sounds_hashtable[hash];
	sounds_hashtable[hash] = s;

	return s;
}

void	sound_free(sound_t *s)
{
	unsigned int hash;
	sound_t *s1, *s2;

	hash = sound_hash(s->path);
	s2 = NULL;
	for (s1 = sounds_hashtable[hash]; s1 != NULL; s1 = s1->next) {
		if (!strcmp(s1->path, s->path)) {
			break;
		}
		s2 = s1;
	}
	if (s1 == NULL) return;
	if (s2 != NULL) s2->next = s1->next;
	SDL_FreeWAV(s1->buf);
	memset(s1, 0, sizeof(sound_t));
}

static void sound_convert(SDL_AudioSpec *src_spec, SDL_AudioSpec *dst_spec,
	Uint8 *src_buf, Uint32 src_len, Uint8 **dst_buf, Uint32 *dst_len)
{
	SDL_AudioCVT cvt;

	SDL_BuildAudioCVT(&cvt, src_spec->format, src_spec->channels,
		src_spec->freq, dst_spec->format, dst_spec->channels,
		dst_spec->freq);

	if (cvt.needed == 0) {
		*dst_buf = src_buf;
		*dst_len = src_len;
		return;
	}

	cvt.len = src_len;
	cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
	memcpy(cvt.buf, src_buf, src_len);
	if (SDL_ConvertAudio(&cvt)) {
		FATAL(SDL_GetError());
	}
	*dst_buf = cvt.buf;
	*dst_len = cvt.len;
}

sound_t	*sound_load(const char *file)
{
	sound_t	*s;
	SDL_AudioSpec wav_spec, *src_spec;
	Uint32 len, newlen;
	Uint8 *newbuf;

	extern SDL_AudioSpec audio_spec;

	s = sound_alloc(file);
	if (s->buf != NULL) {
		return s;
	}

	wav_spec = audio_spec;
	src_spec = SDL_LoadWAV(file, &wav_spec, &s->buf, &s->len);
	if (src_spec == NULL)
		FATAL(SDL_GetError());

	sound_convert(src_spec, &audio_spec, s->buf, s->len, &newbuf, &newlen);
	if (s->buf != newbuf) {
		SDL_FreeWAV(s->buf);
		s->buf = newbuf;
		s->len = newlen;
	}

	switch (audio_spec.format) {
	case AUDIO_U8:
		{
			Uint8 *p;
			
			len = s->len;
			for (p = s->buf; len--; ++p) {
				*p /= MAX_CHANNELS;
			}
		}
		break;
	case AUDIO_S16SYS:
		{
			Sint16 *p;
	
			s->len /= 2;	
			len = s->len; 
			for (p = (Sint16*)s->buf; len--; ++p) {
				*p /= MAX_CHANNELS;
			}
		}
		break;
	case AUDIO_S32SYS:
		{
			Sint32 *p;
			
			s->len /= 4;
			len = s->len;
			for (p = (Sint32*)s->buf; len--; ++p) {
				*p /= MAX_CHANNELS;
			}
		}
		break;
	}
			
	return s;
}

channel_t	*sound_play(sound_t *s, int loop)
{
	channel_t *ch;

	audio_lock();

	ch = channel_alloc();
	ch->sound = s;
	ch->offset = 0;
	if (loop) ch->flags = CHANNEL_LOOP;
	else ch->flags = 0;

	audio_unlock();

	return ch;
}

void	sound_pause(channel_t *c)
{
	audio_lock();
	c->flags |= CHANNEL_PAUSED;
	audio_unlock();
}

void	sound_resume(channel_t *c)
{
	audio_lock();
	c->flags &= ~CHANNEL_PAUSED;
	audio_unlock();
}

void	sound_stop(channel_t *c)
{
	audio_lock();
	channel_free(c);
	audio_unlock();	
}


/*
 * This is really ugly and slow.
 */
void	audio_callback(void *cookie, Uint8 *stream, int len)
{
	int i, n;
	extern var_t *s_volume, *s_mute;

	memset(stream, audio_spec.silence, len);

	if (audio_spec.format == AUDIO_U8) {
		for (i = 0; i < MAX_CHANNELS; ++i) {
			if (channels[i].sound == NULL) continue;
			if (channels[i].flags & CHANNEL_PAUSED) continue;
			for (n = 0; n < len; ++n) {
				if (s_mute->integer == 0) {
					stream[n] += channels[i].sound->buf[channels[i].offset] * s_volume->real;
				}
				++channels[i].offset;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
						break;
					}
				}
			}
		}
	} else if (audio_spec.format == AUDIO_S16SYS) {
		len /= 2;
		for (i = 0; i < MAX_CHANNELS; ++i) {
			if (channels[i].sound == NULL) continue;
			if (channels[i].flags & CHANNEL_PAUSED) continue;
			for (n = 0; n < len; ++n) {
				if (s_mute->integer == 0) {
					((Sint16*)stream)[n] += 
						((Sint16*)channels[i].sound->buf)[channels[i].offset] * s_volume->real;
				}
				++channels[i].offset;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
						break;
					}
				}
			}
		}
	} else if (audio_spec.format == AUDIO_S32SYS) {
		len /= 4;
		for (i = 0; i < MAX_CHANNELS; ++i) {
			if (channels[i].sound == NULL) continue;
			if (channels[i].flags & CHANNEL_PAUSED) continue;
			for (n = 0; n < len; ++n) {
				if (s_mute->integer == 0) {
					((Sint32*)stream)[n] +=
						((Sint32*)channels[i].sound->buf)[channels[i].offset] * s_volume->real;
				}
				++channels[i].offset;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
						break;
					}
				}
			}
		}
	} 
}

