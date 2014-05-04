#include "common.h"
#include "video/video.h"

SDL_Texture	*image_load(const char *file)
{
	SDL_Texture *texture;
	SDL_Surface *surface;

	surface = IMG_Load(file);
	if (surface == NULL) {
		ERROR("Failed to load image: %s\n%s", file, IMG_GetError());
	}
	
	texture = SDL_CreateTextureFromSurface(v_renderer, surface);
	if (texture == NULL) {
		ERROR("Failed to create texture from image: %s\n%s", file, SDL_GetError());
	}
	SDL_FreeSurface(surface);

	return texture;	
}

void	image_colorize(SDL_Texture *image, Uint32 hue)
{
	SDL_Color color;

	hue = hton32(hue);
	color = *(SDL_Color*)&hue;

	SDL_SetTextureColorMod(image, color.r, color.g, color.b);
}

