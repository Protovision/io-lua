#include "common.h"

const char	*platform;
var_t		*c_basezip, *c_gamezip, *c_datapath, *c_fps,
		*c_fontsize, *c_fontfamily, *c_fgcolor, *c_bgcolor,
		*c_title, *c_memblocks, *c_memsize;

gameVar_t common_vars[] = {
	{ &c_title, "title", "io-lua application" },
	{ &c_fps, "fps", "85" },
	{ &c_fontsize, "fontsize", "12" },
	{ &c_fontfamily, "fontfamily", "FONT_SANS" },
	{ &c_fgcolor, "fgcolor", "0x000000FF" },
	{ &c_bgcolor, "bgcolor", "0xFFFFFFFF" },
	{ &c_memblocks, "memblocks", "4096" },
	{ &c_memsize, "memsize", "65536" },
	{ NULL, NULL, NULL },
};

void	common_init(int argc, char *argv[])
{
	int i;
	char *s;

	SDL_Init(0);
	var_init();

	for (i = 1; i != argc; ++i) {
		if (!strcmp(argv[i], "+set")) {
			if (i + 2 >= argc) break;
			var_set(argv[i+1], argv[i+2]);
			i += 2;	
		}
	}

	platform = SDL_GetPlatform();
	s = SDL_GetBasePath();

	c_basezip = var_get("base");
	if (c_basezip == NULL) {
		c_basezip = var_set("base", va("%sbase.zip", s));
	}

	c_gamezip = var_get("game");
	if (c_gamezip == NULL) {
		c_gamezip = var_set("game", va("%sgame.zip", s));
	}

	SDL_free(s);

	c_datapath = var_get("datapath");
	if (c_datapath == NULL) {
		s = SDL_GetPrefPath("protovision", "io-lua");
		s[strlen(s)-1] = 0;				
		c_datapath = var_set("datapath", s);
		SDL_free(s);
	}

	var_load(common_vars);

	pool_init(c_memblocks->integer, c_memsize->integer);	
	unz_init();
	event_init();
	font_init();
	image_init();
	script_init();
	trap_init();
}

void	common_shutdown()
{
	trap_shutdown();
	script_shutdown();
	image_shutdown();
	font_shutdown();
	event_shutdown();
	unz_shutdown();
	pool_shutdown();
	var_shutdown();
}

