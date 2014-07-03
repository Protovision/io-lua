#include "video.h"

static var_t		*v_driver;
static var_t		*v_renderDriver;
static var_t		*v_width;
static var_t		*v_height;
static var_t		*v_fullscreen;
static var_t		*v_brightness;
static var_t		*v_grab;

SDL_Window	*v_window;
SDL_Renderer	*v_renderer;

gameVar_t	video_vars[] = {
	{ &v_width, "v_width", "640" },
	{ &v_height, "v_height", "480" },
	{ &v_fullscreen, "v_fullscreen", "0" },
	{ &v_brightness, "v_brightness", "1.0" },
	{ &v_grab, "v_grab", "0" },
	{ NULL, NULL, NULL }
};

void	video_init()
{
	const char *driver_name;

	var_load(video_vars);

	v_driver = var_get("v_driver");
	v_renderDriver = var_get("v_renderer");
	
	if (strcmp(platform, "Mac OS X") == 0) {
		if (v_driver == NULL)
			v_driver = var_set("v_driver", "0");
		if (v_renderDriver == NULL) 
			v_renderDriver = var_set("v_renderer", "1");
	} else if (strcmp(platform, "Windows") == 0 || strcmp(platform, "Linux") == 0) {
		if (v_driver == NULL)
			v_driver = var_set("v_driver", "0");
		if (v_renderDriver == NULL)
			v_renderDriver = var_set("v_renderer", "-1");
	}

	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
			FATAL(SDL_GetError());
		}
	}

	driver_name = SDL_GetVideoDriver(v_driver->integer);
	if (SDL_VideoInit(driver_name)) {
		FATAL(SDL_GetError());
	}

	v_window = SDL_CreateWindow(
		c_title->string, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		v_width->integer, v_height->integer, SDL_WINDOW_SHOWN);
	if (v_window == NULL) {
		FATAL(SDL_GetError());
	}

	v_renderer = SDL_CreateRenderer(v_window, v_renderDriver->integer, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (v_renderer == NULL) {
		FATAL(SDL_GetError());
	}

	SDL_SetRenderDrawBlendMode(v_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetWindowGrab(v_window, v_grab->integer);
	SDL_SetWindowFullscreen(v_window, v_fullscreen->integer ? SDL_WINDOW_FULLSCREEN : 0);
}

void	video_shutdown()
{
	if (v_renderer != NULL) {
		SDL_DestroyRenderer(v_renderer);
		v_renderer = NULL;
	}
	if (v_window != NULL) {
		SDL_DestroyWindow(v_window);
		v_window = NULL;
	}
	SDL_VideoQuit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void	video_setFullscreen(int boolean)
{
	SDL_SetWindowFullscreen(v_window, boolean ? SDL_WINDOW_FULLSCREEN : 0);
	var_set("v_fullscreen", va("%d", boolean));
}

void	video_setGrab(int boolean)
{
	SDL_SetWindowGrab(v_window, boolean);
	var_set("v_grab", va("%d", boolean));
}
