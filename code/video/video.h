#ifndef SCR_H
#define SCR_H

#include "common.h"

void	video_init();
void	video_shutdown();

void	video_setFullscreen(int boolean);
void	video_setGrab(int boolean);
void	video_getWindowSize(int *x, int *y);
void	video_drawText(int x, int y, const char *text, FONT *font, unsigned int color);
void	video_drawBackground(IMAGE *img);
void	video_drawImage(int x, int y, IMAGE *image, int w, int h);
void	video_drawClip(int dstx, int dsty, IMAGE *image, int srcx, int srcy, int srcw, int srch, int dstw, int dsth);
void	video_render();
void	video_setColor(unsigned long int color);
void	video_clear();
void	video_drawLine(int x1, int y1, int x2, int y2);
void	video_drawRect(int x, int y, int w, int h);
void	video_fillRect(int x, int y, int w, int h);
void	video_drawPoint(int x, int y);


#endif
