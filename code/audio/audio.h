#ifndef AUDIO_H 
#define AUDIO_H 

#include "common/common.h"

void	audio_init();
void	audio_shutdown();

void	audio_pause();
void	audio_resume();
void	audio_stop();

struct sound_s {
	Uint8		*buf;
	Uint32		len;
	char		path[MAX_PATH];
	struct sound_s	*next;	
};

typedef struct sound_s	sound_t;

struct channel_s {
	sound_t		*sound;
	int		offset;
	int		flags;
	struct channel_s *next_free;
};

typedef struct channel_s	channel_t;

void		sound_init();
void		sound_shutdown();

sound_t		* sound_load(const char *file);
void		sound_free(sound_t *s);
channel_t	* sound_play(sound_t *s, int loop);
void		sound_pause(channel_t *c);
void		sound_resume(channel_t *c);
void		sound_stop(channel_t *c);



#endif
