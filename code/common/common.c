#include "common.h"

char *base_path;
var_t *c_game, *c_fps;

void	common_init(int argc, char *argv[])
{
	int i;

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

	c_game = var_get("c_game");
	if (c_game == NULL) {
		c_game = var_set("c_game", "default");
	}

	c_fps = var_get("c_fps");
	if (c_fps == NULL) {
		c_fps = var_set("c_fps", "85");
	}

	base_path = SDL_GetPrefPath("protovision", "io-lua");

	if (!sys_exists( va("%sconstants.lua", base_path) )) {
		sys_copy("base/constants.lua", base_path);
	}
	if (!sys_exists( va("%sdefault", base_path) )) {
		sys_copy("base/default", base_path);
	}
	
	script_init();
	trap_init();
	font_init();
	image_init();
	event_init();


}

void	common_shutdown()
{
	SDL_free(base_path);

	event_shutdown();
	image_shutdown();
	font_shutdown();
	trap_shutdown();
	script_shutdown();
	var_shutdown();
	mem_shutdown();
}

