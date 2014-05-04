#include "common.h"

char *base_path;
static int usepool, smallpool_chunks, smallpool_size, bigpool_chunks, bigpool_size;
static const char *game;

struct {
	const char *name;
	char type;
	void *var;
	const char *default_value;
} init_vars[] = {
	{ "game", 'c', (void*)&game, "default" },
	{ "usepool", 'i', &usepool, "1" },
	{ "smallpoolchunks", 'i', &smallpool_chunks, "2048"},
	{ "smallpoolsize", 'i', &smallpool_size, "512" },
	{ "bigpoolchunks", 'i', &bigpool_chunks, "63" },
	{ "bigpoolsize", 'i', &bigpool_size, "1048576" },
	{ NULL, 0, NULL, NULL }
};

void	common_set(const char *key, const char *value)
{
	int i;
	for (i = 0; init_vars[i].name != NULL; ++i) {
		if (strcmp(init_vars[i].name, key)) continue;
		switch (init_vars[i].type) {
		case 'i':
			*(int*)init_vars[i].var = atoi(value);
			break;
		case 'f':
			*(double*)init_vars[i].var = atof(value);
			break;	
		case 's':
			strcpy((char*)init_vars[i].var, value);
			break;
		case 'c':
			*(const char**)init_vars[i].var = value;
		}
		break;
	}
}

void	common_parseArgs(int argc, char *argv[])
{
	int i;
	const char *key, *value;
	
	for (i = 0; init_vars[i].name != NULL; ++i) {
		switch (init_vars[i].type) {
		case 'i':
			*(int*)init_vars[i].var = atoi(init_vars[i].default_value);
			break;
		case 'f':
			*(double*)init_vars[i].var = atof(init_vars[i].default_value);
			break;	
		case 's':
			strcpy((char*)init_vars[i].var, init_vars[i].default_value);
			break;
		case 'c':
			*(const char**)init_vars[i].var = init_vars[i].default_value;
		}
	}

	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "+set") == 0) {
			key = argv[++i];
			value = argv[++i];
			common_set(key, value);
		}
	}
}

void	common_init(int argc, char *argv[])
{
	common_parseArgs(argc, argv);

	mem_init(smallpool_chunks, smallpool_size,
		bigpool_chunks, bigpool_size);
	var_init();
	script_init(usepool);
	trap_init();
	font_init();
	image_init();
	event_init();

	c_game = var_set("c_game", game);

	c_fps = var_get("c_fps");
	if (c_fps == NULL) c_fps = var_set("c_fps", "85");

	base_path = SDL_GetPrefPath("protovision", "io-lua");

	if (!sys_exists( va("%sconstants.lua", base_path) )) {
		sys_copy("base/constants.lua", base_path);
	}
	if (!sys_exists( va("%sdefault", base_path) )) {
		sys_copy("base/default", base_path);
	}
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

