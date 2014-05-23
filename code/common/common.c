#include "common.h"

char	*_basepath;
var_t	*c_gamepath, *c_datapath, *c_fps;

void	common_init(int argc, char *argv[])
{
	int i;
	char *s;

	SDL_Init(0);
	mem_init();
	var_init();

	for (i = 1; i != argc; ++i) {
		if (!strcmp(argv[i], "+set")) {
			if (i + 2 >= argc) break;
			var_set(argv[i+1], argv[i+2]);
			i += 2;	
		}
	}

	_basepath = SDL_GetBasePath();
	
	c_gamepath = var_get("gamepath");
	if (c_gamepath == NULL) {
		c_gamepath = var_set("gamepath", va("%sgame", _basepath));
	}

	c_datapath = var_get("datapath");
	if (c_datapath == NULL) {
		s = SDL_GetPrefPath("protovision", "io-lua");
		c_datapath = var_set("datapath", s);
		SDL_free(s);
	}

	c_fps = var_get("fps");
	if (c_fps == NULL) {
		c_fps = var_set("fps", "85");
	}

	script_init();
	trap_init();
	font_init();
	image_init();
	event_init();
}

void	common_shutdown()
{
	SDL_free(_basepath);
	event_shutdown();
	image_shutdown();
	font_shutdown();
	trap_shutdown();
	script_shutdown();
	var_shutdown();
	mem_shutdown();
}

