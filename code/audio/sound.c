#include "audio.h"

SOUND	*sound_load(const char *file)
{
	SOUND *s;
	
	s = Mix_LoadWAV_RW(unz_open(file), 1);
	if (s == NULL)
		FATAL(Mix_GetError());
	
	return s;	
}

CHANNEL	sound_play(SOUND *s, int loop)
{
	CHANNEL c;

	c = Mix_PlayChannel(-1, s, loop ? -1 : 0);
	if (c == -1)
		FATAL(Mix_GetError());

	return c;
}
