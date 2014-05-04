#ifndef SCR_H
#define SCR_H

#include "common/common.h"

extern SDL_Window	*v_window;
extern SDL_Renderer	*v_renderer;
	
void	video_init();
void	video_shutdown();

void	video_drawText(int x, int y, const char *text, FONT *font, unsigned int color);
void	video_drawBackground(IMAGE *img);
void	video_drawImage(int x, int y, IMAGE *image);
void	video_render();
void	video_setColor(unsigned long int color);
void	video_clear();
void	video_drawLine(int x1, int y1, int x2, int y2);
void	video_drawRect(int x, int y, int h, int w);
void	video_fillRect(int x, int y, int h, int w);
void	video_drawPoint(int x, int y);


#endif
