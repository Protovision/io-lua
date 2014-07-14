#include "audio.h"

SOUND	*sound_load(const char *file)
{
	SOUND *s;
	
	s = Mix_LoadWAV_RW(unz_open(file), 1);
	if (s == NULL)
		FATAL(Mix_GetError());
	
	return s;	
}

CHANNEL	sound_play(SOUND *s, int loop, int fadein)
{
	CHANNEL c;

	if (fadein) {
		c = Mix_FadeInChannel(-1, s, loop ? -1 : 0, fadein * 1000);
	} else {
		c = Mix_PlayChannel(-1, s, loop ? -1 : 0);
	}

	if (c == -1)
		FATAL(Mix_GetError());

	return c;
}

MUSIC	*music_load(const char *file)
{
	MUSIC *m;

	m = Mix_LoadMUS_RW(unz_open(file), 1);
	if (m == NULL)
		FATAL(Mix_GetError());

	return m;
}

void	music_play(MUSIC *m, int loop, int fadein)
{
	int result;

	if (fadein) {
		result = Mix_FadeInMusic(m, loop ? -1 : 0, fadein * 1000);
	} else {
		result = Mix_PlayMusic(m, loop ? - 1 : 0);
	}

	if (result == -1)
		FATAL(Mix_GetError());
}
