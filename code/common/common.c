#include "common.h"

const char	*platform;
var_t		*c_basepath, *c_gamepath, *c_datapath, *c_fps,
		*c_fontsize, *c_fontfamily, *c_fgcolor, *c_bgcolor,
		*c_title;

gameVar_t common_vars[] = {
	{ &c_title, "title", "io-lua application" },
	{ &c_fps, "fps", "85" },
	{ &c_fontsize, "fontsize", "12" },
	{ &c_fontfamily, "fontfamily", "FONT_SANS" },
	{ &c_fgcolor, "fgcolor", "0x000000FF" },
	{ &c_bgcolor, "bgcolor", "0xFFFFFFFF" },
	{ NULL, NULL, NULL },
};

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

	platform = SDL_GetPlatform();
	s = SDL_GetBasePath();

	c_basepath = var_get("basepath");
	if (c_basepath == NULL) {
		c_basepath = var_set("basepath", va("%sbase", s));
	}

	c_gamepath = var_get("gamepath");
	if (c_gamepath == NULL) {
		c_gamepath = var_set("gamepath", va("%sgame", s));
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
	var_shutdown();
	mem_shutdown();
}

