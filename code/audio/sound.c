#include "audio.h"

#define	MAX_SOUNDS	256
#define	MAX_SOUNDS_HASH	128
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

static int	sound_hash(const char *file)
{
	int	i, hash;
	char	ch;

	hash = i = 0;
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
	int i, hash;
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
		ERROR("Maximum number of loaded sounds exceeded");
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
	int hash;
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

sound_t	*sound_load(const char *file)
{
	sound_t	*s;
	SDL_AudioSpec wav_spec;
	Uint32 len;

	extern SDL_AudioSpec audio_spec;

	s = sound_alloc(file);
	if (s->buf != NULL) {
		return s;
	}

	wav_spec = audio_spec;
	if (SDL_LoadWAV(file, &wav_spec, &s->buf, &s->len) == NULL)
		ERROR("Error loading sound: %s", file);

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
	extern var_t *s_volume;
	
	memset(stream, audio_spec.silence, len);

	if (audio_spec.format == AUDIO_U8) {
		for (i = 0; i < MAX_CHANNELS; ++i) {
			if (channels[i].sound == NULL) continue;
			if (channels[i].flags & CHANNEL_PAUSED) continue;
			for (n = 0; n < len; ++n) {
				stream[n] += channels[i].sound->buf[channels[i].offset++] * s_volume->real;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
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
				((Sint16*)stream)[n] += ((Sint16*)channels[i].sound->buf)[channels[i].offset++] * s_volume->real;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
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
				((Sint32*)stream)[n] += ((Sint32*)channels[i].sound->buf)[channels[i].offset++] * s_volume->real;
				if (channels[i].offset >= channels[i].sound->len) {
					if (channels[i].flags & CHANNEL_LOOP) {
						channels[i].offset = 0;
					} else {
						channel_free(&channels[i]);
					}
				}
			}
		}
	} 
}

