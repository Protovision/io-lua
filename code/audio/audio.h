#ifndef AUDIO_H 
#define AUDIO_H 

#include "common.h"

void	audio_init();
void	audio_shutdown();

#define audio_pause()		Mix_Pause(-1)
#define audio_resume()		Mix_Resume(-1)
#define audio_stop()		Mix_HaltChannel(-1)
#define audio_set_volume(v)	Mix_Volume(-1, (v))
#define audio_get_volume()	Mix_Volume(-1, -1)

void	audio_mute();
void	audio_unmute();

#define SOUND	Mix_Chunk
#define	CHANNEL	int

#define sound_free(s)	Mix_FreeChunk(s)
#define	sound_pause(c)	Mix_Pause(c)
#define sound_resume(c)	Mix_Resume(c)
#define sound_stop(c)	Mix_HaltChannel(c)

SOUND		*sound_load(const char *file);
CHANNEL		sound_play(SOUND *s, int loop);

#endif
