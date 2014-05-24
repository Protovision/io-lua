#include "common.h"
#include "video/video.h"

SDL_Texture	*image_load(const char *file, int w, int h)
{
	SDL_Texture *texture;
	SDL_Surface *surface, *scaled, *s;
	SDL_Rect r;
	extern SDL_Renderer *v_renderer;

	surface = IMG_Load(file);
	if (surface == NULL) {
		ERROR(IMG_GetError());
	}

	if (w && h) {
		r.x = 0;
		r.y = 0;
		r.w = w;
		r.h = h;	
		scaled = SDL_CreateRGBSurface(0, w, h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		SDL_BlitScaled(surface, NULL, scaled, &r);
		s = surface;
		surface = scaled;
		SDL_FreeSurface(s);	
	} 
	
	texture = SDL_CreateTextureFromSurface(v_renderer, surface);
	if (texture == NULL) {
		ERROR(SDL_GetError());
	}
	SDL_FreeSurface(surface);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	return texture;	
}

void	image_colorize(SDL_Texture *image, Uint32 hue)
{
	SDL_Color color;

	hue = hton32(hue);
	color = *(SDL_Color*)&hue;

	SDL_SetTextureColorMod(image, color.r, color.g, color.b);
}

