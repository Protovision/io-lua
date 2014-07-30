#include "common.h"
#include "video.h"

SDL_Texture	*image_load(const char *file)
{
	SDL_Texture *texture;
	SDL_Surface *surface;
	extern SDL_Renderer *v_renderer;

	surface = IMG_Load_RW(unz_open(file), 1);
	if (surface == NULL)
		FATAL(IMG_GetError());
	
	texture = SDL_CreateTextureFromSurface(v_renderer, surface);
	if (texture == NULL)
		FATAL(SDL_GetError());

	SDL_FreeSurface(surface);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	return texture;	
}

void	image_query(SDL_Texture *image, int *w, int *h)
{
	Uint32 format;
	int access;

	SDL_QueryTexture(image, &format, &access, w, h);
}

void	image_colorize(SDL_Texture *image, Uint32 hue)
{
	SDL_Color color;

	hue = hton32(hue);
	color = *(SDL_Color*)&hue;

	SDL_SetTextureColorMod(image, color.r, color.g, color.b);
}

void	image_fade(SDL_Texture *image, int alpha)
{
	if (SDL_SetTextureAlphaMod(image, (Uint8)alpha))
		FATAL(SDL_GetError());
}
