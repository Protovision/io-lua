#ifndef AUDIO_H 
#define AUDIO_H 

#include "common.h"

void	audio_init();
void	audio_shutdown();

#define SOUND	Mix_Chunk
#define	CHANNEL	int

#define sound_free(s)		Mix_FreeChunk(s)
#define	sound_pause(c)		Mix_Pause(c)
#define sound_resume(c)		Mix_Resume(c)
#define sound_stop(c)		Mix_HaltChannel(c)
#define sound_set_volume(c, v)	Mix_Volume(c, v)
#define sound_get_volume(c)	Mix_Volume(c, -1)
#define sound_paused(c)		Mix_Paused(c)
#define sound_playing(c)	Mix_Playing(c)
#define sound_fade(c, f)	Mix_FadeOutChannel(c, (f) * 1000)

SOUND	*sound_load(const char *file);
CHANNEL	sound_play(SOUND *s, int loop, int fadein);

#define MUSIC	Mix_Music

#define music_free(m)		Mix_FreeMusic(m)
#define music_pause()		Mix_PauseMusic()
#define music_resume()		Mix_ResumeMusic()
#define music_rewind()		Mix_RewindMusic()
#define music_seek(s)		Mix_SetMusicPosition(s)
#define music_stop()		Mix_HaltMusic()
#define music_fade(f)		Mix_FadeOutMusic((f) * 1000)
#define music_paused()		Mix_PausedMusic()
#define music_playing()		Mix_PlayingMusic()
#define music_get_volume()	Mix_VolumeMusic(-1)
#define music_set_volume(v)	Mix_VolumeMusic(v)

MUSIC	*music_load(const char *file);
void	music_play(MUSIC *m, int loop, int fadein);

#endif
