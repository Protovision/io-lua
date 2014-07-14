#include "common.h"

TTF_Font	*font_load(const char *filename, int size)
{
	TTF_Font *font;

	font = TTF_OpenFontRW(unz_open(filename), 1, size);

	if (font == NULL)
		FATAL("Failed to load font %s: %s", filename, TTF_GetError());

	return font;
}

void	font_text_size(TTF_Font *font, const char *text, int *w, int *h)
{
	int res;

	res = TTF_SizeText(font, text, w, h);
	if (res == -1)
		FATAL(TTF_GetError());
	
}
