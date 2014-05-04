#include "video.h"

void	video_drawBackground(IMAGE *image)
{
	SDL_RenderCopy(v_renderer, image, NULL, NULL);
}

void	video_drawImage(int x, int y, IMAGE *image)
{
	SDL_Rect r;

	r.x = x;
	r.y = y;
	SDL_QueryTexture(image, NULL, NULL, &r.w, &r.h);
	SDL_RenderCopy(v_renderer, image, NULL, &r);	
}

void	video_drawText(int x, int y, const char *text, FONT *font, unsigned int color)
{
	SDL_Surface *surface;
	SDL_Rect rect;
	SDL_Texture *texture;

	color = hton32(color);
	surface = TTF_RenderText_Blended(font, text, *(SDL_Color*)&color);
	if (surface == NULL) {
		ERROR("Failed to render text: %s", TTF_GetError());
	}

	texture = SDL_CreateTextureFromSurface(v_renderer, surface);
	if (texture == NULL) {
		ERROR("Failed to create texture: %s", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	rect.x = x;
	rect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(v_renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

void	video_render()
{
	SDL_RenderPresent(v_renderer);
}

void	video_setColor(unsigned long int color)
{
	SDL_Color c;
	
	color = hton32(color);
	c = *(SDL_Color*)&color;
	SDL_SetRenderDrawColor(v_renderer, c.r, c.g, c.b, c.a);
}

void	video_clear()
{
	SDL_RenderClear(v_renderer);
}

void	video_drawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(v_renderer, x1, y1, x2, y2);
}

void	video_drawRect(int x, int y, int h, int w)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.h = h;
	r.w = w;
	
	SDL_RenderDrawRect(v_renderer, &r);
}

void	video_fillRect(int x, int y, int h, int w)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.h = h;
	r.w = w;
	
	SDL_RenderFillRect(v_renderer, &r);
}

void	video_drawPoint(int x, int y)
{
	SDL_RenderDrawPoint(v_renderer, x, y);
}
