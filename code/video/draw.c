#include "video.h"

void	video_drawBackground(IMAGE *image)
{
	extern SDL_Renderer *v_renderer;

	SDL_RenderCopy(v_renderer, image, NULL, NULL);
}

void	video_drawImage(int x, int y, IMAGE *image, int w, int h)
{
	SDL_Rect r;
	extern SDL_Renderer *v_renderer;

	r.x = x;
	r.y = y;
	if (w && h) {
		r.w = w;
		r.h = h;
	} else {
		SDL_QueryTexture(image, NULL, NULL, &r.w, &r.h);
	}
	SDL_RenderCopy(v_renderer, image, NULL, &r);	
}

void	video_drawClip(int dstx, int dsty, IMAGE *image, int x, int y, int w, int h, int scalew, int scaleh)
{
	SDL_Rect src, dst;
	extern SDL_Renderer *v_renderer;

	src.x = x;
	src.y = y;
	src.w = w;
	src.h = h;
	
	dst.x = dstx;
	dst.y = dsty;
	if (scalew && scaleh) {
		dst.w = scalew;
		dst.h = scaleh;
	} else {
		dst.w = w;
		dst.h = h;
	}
	
	SDL_RenderCopy(v_renderer, image, &src, &dst);	
}

void	video_drawText(int x, int y, const char *text, FONT *font, const char *color)
{
	SDL_Surface *surface;
	SDL_Rect rect;
	SDL_Texture *texture;
	unsigned int c;	
	extern SDL_Renderer *v_renderer;

	sscanf(color, "%x", &c);	
	c = hton32(c);
	surface = TTF_RenderText_Blended(font, text, *(SDL_Color*)&c);
	if (surface == NULL) {
		FATAL(TTF_GetError());
	}

	texture = SDL_CreateTextureFromSurface(v_renderer, surface);
	if (texture == NULL) {
		FATAL(SDL_GetError());
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
	extern SDL_Renderer *v_renderer;

	SDL_RenderPresent(v_renderer);
}

void	video_setColor(unsigned long int color)
{
	SDL_Color c;
	extern SDL_Renderer *v_renderer;
	
	color = hton32(color);
	c = *(SDL_Color*)&color;
	SDL_SetRenderDrawColor(v_renderer, c.r, c.g, c.b, c.a);
}

void	video_clear()
{
	extern SDL_Renderer *v_renderer;

	SDL_RenderClear(v_renderer);
}

void	video_drawLine(int x1, int y1, int x2, int y2)
{
	extern SDL_Renderer *v_renderer;

	SDL_RenderDrawLine(v_renderer, x1, y1, x2, y2);
}

void	video_drawRect(int x, int y, int w, int h)
{
	SDL_Rect r;
	extern SDL_Renderer *v_renderer;

	r.x = x;
	r.y = y;
	r.h = h;
	r.w = w;
	
	SDL_RenderDrawRect(v_renderer, &r);
}

void	video_fillRect(int x, int y, int w, int h)
{
	SDL_Rect r;
	extern SDL_Renderer *v_renderer;

	r.x = x;
	r.y = y;
	r.h = h;
	r.w = w;
	
	SDL_RenderFillRect(v_renderer, &r);
}

void	video_drawPoint(int x, int y)
{
	extern SDL_Renderer *v_renderer;

	SDL_RenderDrawPoint(v_renderer, x, y);
}

void	video_getWindowSize(int *x, int *y)
{
	extern SDL_Window *v_window;

	SDL_GetWindowSize(v_window, x, y);	
}
