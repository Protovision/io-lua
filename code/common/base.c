#include "common.h"
#include "audio.h"
#include "video.h"

typedef struct {
	const char *name;
	const char *file;
} resource_t; 

typedef void (*resource_loader)(const char *name, const char *file);
typedef void (*resource_cleaner)(const char *name);

typedef struct {
	resource_t		*resources;
	resource_loader		load;
	resource_cleaner	cleanup;	
} game_resources_t;

static resource_t fonts[] = {
	{ "FONT_SANS", "fonts/FreeSans.ttf" },
	{ "FONT_SANS_BOLD", "fonts/FreeSansBold.ttf" },
	{ "FONT_SANS_ITALIC", "fonts/FreeSansOblique.ttf" },
	{ "FONT_SERIF", "fonts/FreeSerif.ttf" },
	{ "FONT_SERIF_BOLD", "fonts/FreeSerifBold.ttf" },
	{ "FONT_SERIF_ITALIC", "fonts/FreeSerifItalic.ttf" },
	{ "FONT_MONO", "fonts/FreeMono.ttf" },
	{ "FONT_MONO_BOLD", "fonts/FreeMonoBold.ttf" },
	{ NULL, NULL }
};

static resource_t images[] = {
	{ NULL, NULL }
};

static resource_t sounds[] = {
	{ NULL, NULL }
};

static void export_font(const char *name, const char *file)
{
	script_export_pointer(name, font_load(file, c_fontsize->integer));
}

static void export_image(const char *name, const char *file)
{
	script_export_pointer(name, image_load(file));
}

static void export_sound(const char *name, const char *file)
{
	script_export_pointer(name, sound_load(file));
}

static void cleanup_font(const char *name)
{
	font_free(script_import_pointer(name));
}

static void cleanup_image(const char *name)
{
	image_free(script_import_pointer(name));	
}

static void cleanup_sound(const char *name)
{
	sound_free(script_import_pointer(name));
}

static game_resources_t game_resources[] = {
	{ fonts, export_font, cleanup_font },
	{ images, export_image, cleanup_image },
	{ sounds, export_sound, cleanup_sound },
	{ NULL, NULL, NULL }
};

static const char *game_scripts[] = {
	"scripts/constants.lua",
	NULL
};

void	base_init()
{
	game_resources_t *r;
	resource_t *s;
	int i;
	unsigned int color;

	for (r = game_resources; r->resources != NULL; ++r) {
		for (s = r->resources; s->name != NULL; ++s) {
			r->load(s->name, s->file);
		}	
	}

	for (i = 0; game_scripts[i] != NULL; ++i) {
		script_load(game_scripts[i]);
	}

	sscanf(c_bgcolor->string, "%x", &color);
	video_setColor(color);
	video_clear();
	sscanf(c_fgcolor->string, "%x", &color);
	video_setColor(color);
}

void	base_shutdown()
{
	game_resources_t *r;
	resource_t *s;
	
	for (r = game_resources; r->resources != NULL; ++r) {
		for (s = r->resources; s->name != NULL; ++s) {
			r->cleanup(s->name);
		}
	}
}
