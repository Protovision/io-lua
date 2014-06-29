#include "common.h"

TTF_Font	*font_load(const char *filename, int size)
{
	TTF_Font *font;

	font = TTF_OpenFontRW(unz_open(filename), 1, size);

	if (font == NULL)
		FATAL("Failed to load font %s: %s", filename, TTF_GetError());

	return font;
}
