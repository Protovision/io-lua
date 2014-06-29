#include "common.h"

SDL_Cursor *cursor_load(const char *filename, int hotx, int hoty)
{
	SDL_Surface *surface;
	SDL_Cursor *cursor;

	surface = IMG_Load_RW(unz_open(filename), 1);
	if (surface == NULL)
		FATAL(SDL_GetError());

	cursor = SDL_CreateColorCursor(surface, hotx, hoty);
	if (cursor == NULL)
		FATAL(SDL_GetError());

	SDL_FreeSurface(surface);
	
	return cursor;	
}

