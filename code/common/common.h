#ifndef COMMON_H
#define COMMON_H

#define ENABLE_EVENT_MOUSEMOVE	0
#define ENABLE_EVENT_MOUSEWHEEL	0

#define MAX_PATHNAME		256
#define MAX_STRING		32000
#define MAX_FILE		(4*1024*1024)

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifndef _WINDOWS
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MKDIR(P)	mkdir((P),0777)
#define RMDIR(P)	rmdir((P))
#define GETCWD(B,L)	getcwd((B),(L))
#define PATH(P)		(P)

typedef DIR		directory_t;
typedef struct dirent	direntry_t;

#define OPENDIR(P)	(opendir((P)))
#define READDIR(D)	(readdir((D)))
#define DIRENT_NAME(E)	((E)->d_name)
#define CLOSEDIR(D)	(closedir((D)))

#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>

#define MKDIR(P)	(CreateDirectoryA((P), NULL) == 0)
#define RMDIR(P)	(RemoveDirectoryA((P)) == 0)
#define GETCWD(B, N)	(GetCurrentDirectoryA((N), (B)) == 0)

char	*convert_path(const char *path);
#define PATH(P)	(convert_path((P)))

struct directory_s {
	HANDLE dirh;
	WIN32_FIND_DATA data;
	int first_time;
};

typedef struct directory_s	directory_t;
typedef WIN32_FIND_DATA		direntry_t;

directory_t	*win_opendir(const char *path);
direntry_t	*win_readdir(directory_t *dir);
void		win_closedir(directory_t *dir);

#define OPENDIR(P)	(win_opendir((P)))
#define READDIR(D)	(win_readdir((D)))
#define DIRENT_NAME(E)	((E)->cFileName)	
#define CLOSEDIR(D)	(win_closedir((D)))

#endif

#define FATAL(F, ...)	do { \
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", va((F), ##__VA_ARGS__), NULL);	\
	quit(-1);	\
	} while (0);	\



enum {
	BUTTON_LEFT = 1,
	BUTTON_MIDDLE,
	BUTTON_RIGHT
};

enum {
	SCROLL_UP,
	SCROLL_DOWN
};

enum {
	FILESEEK_SET,
	FILESEEK_CUR,
	FILESEEK_END
};

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define hton16(X)	(SDL_Swap16(X))
#define hton32(X)	(SDL_Swap32(X))
#define hton64(X)	(SDL_Swap64(X))
#define ntoh16(X)	(SDL_Swap16(X))
#define ntoh32(X)	(SDL_Swap32(X))
#define ntoh64(X)	(SDL_Swap64(X))
#else
#define hton16(X)	(X)
#define hton32(X)	(X)
#define hton64(X)	(X)
#define ntoh16(X)	(X)
#define ntoh32(X)	(X)
#define ntoh64(X)	(X)
#endif

// main.c
void	quit(int sig);

//frame.c
void	frame();

/* =====================================================
 * common.c
 * =====================================================
 */

void	common_init(int argc, char *argv[]);
void	common_shutdown();

/* =====================================================
 * keys.c
 * =====================================================
 */

#include "keys.h"

KEY	key_decode(SDL_Keysym *key);

/* =====================================================
 * event.c
 * =====================================================
 */

typedef enum {
	EVENT_NONE = 0,
	EVENT_INIT,
	EVENT_SHUTDOWN,
	EVENT_UPDATE,
	EVENT_KEYBOARD,	
	EVENT_MOUSEBUTTON,
	EVENT_MOUSEWHEEL,
	EVENT_MOUSEMOVE
} eventType_t;

struct event {
	eventType_t	type;
	union {
		struct {
			KEY	key;
			int	pressed;
		} key;
		struct {
			int	x;
			int	y;
			int	button;
			int	pressed;		
		} button;

		struct {
			int	dir;
		} scroll;

#if ENABLE_EVENT_MOUSEMOVE == 1
		struct {
			int	x;
			int	y;
		} move;
#endif

	};

};

typedef struct event	event_t;

void	event_init();
void	event_shutdown();
void	event_push(event_t *ev);
void	event_pull(event_t *ev);

/* =====================================================
 * var.c
 * =====================================================
 */

struct var {
	char	*name;
	char	*string;
	float	real;
	int	integer;

	struct var *next, *prev;
};

typedef struct var var_t;

void	var_init();
void	var_shutdown();
var_t	*var_get(const char *varname);
var_t	*var_set(const char *varname, const char *value);

typedef struct {
	var_t		**var;
	const char	*name;
	const char	*default_value;
} gameVar_t;

void	var_load(gameVar_t *vars);


/* =====================================================
 * script.c
 * =====================================================
 */

void	script_init();
void	script_shutdown();

int	script_load(const char *luafile);
void	script_register(const char *cmdname, int (*func)(lua_State*));
void	script_call(const char *func, const char *fmt, ...);

void	script_export_integer(const char *name, int value);
void	script_export_number(const char *name, double value);
void	script_export_string(const char *name, const char *value);
void	script_export_pointer(const char *name, void *value);
void	script_export_boolean(const char *name, int value);

int	script_import_integer(const char *name);
double	script_import_number(const char *name);
const char *script_import_string(const char *name);
void	*script_import_pointer(const char *name);
int	script_import_boolean(const char *name);

/* =====================================================
 * trap.c
 * =====================================================
 */

void	trap_init();
void	trap_shutdown();

/* =====================================================
 * mem.c
 * =====================================================
 */

void	mem_init();
void	mem_shutdown();
void	*mem_alloc(size_t size);
void	*mem_realloc(void *ptr, size_t size);
void	mem_free(void *ptr);
char	*mem_strdup(const char *s);

/* =====================================================
 * font.c
 * =====================================================
 */

#define	FONT	TTF_Font

#define		font_init()		(TTF_Init())
#define		font_shutdown()		(TTF_Quit())
#define		font_free(F)		(TTF_CloseFont((F)))

FONT		*font_load(const char *filename, int size);
void		font_text_size(FONT *f, const char *text, int *w, int *h);

/* =====================================================
 * image.c
 * =====================================================
 */

#define IMAGE	SDL_Texture

#define		image_init()		(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG))
#define		image_shutdown()	(IMG_Quit())
#define		image_free(I)		(SDL_DestroyTexture((I)))
#define		image_fade(I, A)	(SDL_SetTextureAlphaMod((I), (A)))

IMAGE		*image_load(const char *file);
void		image_query(IMAGE *img, int *w, int *h);
void		image_colorize(IMAGE *img, Uint32 hue);

/* =====================================================
 * util.c
 * =====================================================
 */

char	*va(const char *fmt, ...);
char	*pathjoin(const char *base, const char *path);
//#define basepath(p)	(pathjoin(c_basepath->string, (p)))
//#define gamepath(p)	(pathjoin(c_gamepath->string, (p)))
#define datapath(p)	(pathjoin(c_datapath->string, (p)))

/* ====================================================
 * sys/
 * ====================================================
 */

void	sys_copy(const char *from, const char *to);
int	sys_exists(const char *path);
int	sys_isfile(const char *path);
int	sys_isdir(const char *path);

/* ====================================================
 * base.c
 * ====================================================
 */

void	base_init();
void	base_shutdown();

extern	const char *platform;
extern	var_t *c_basezip, *c_gamezip, *c_datapath,
	*c_fps, *c_fontsize, *c_fontfamily, *c_fgcolor, *c_bgcolor,
	*c_title;

/* ====================================================
 * unz.c
 * ====================================================
 */
	
void		unz_init();
void		unz_shutdown();
int		unz_exists(const char *filename);
SDL_RWops	*unz_open(const char *filename);
char		*unz_load(const char *filename);

/* ====================================================
 * cursor.c
 * ====================================================
 */

#define	CURSOR		SDL_Cursor
#define	cursor_get()	(SDL_GetCursor())
#define	cursor_set(C)	(SDL_SetCursor((C)))
#define	cursor_free(C)	(SDL_FreeCursor((C)))

CURSOR	*cursor_load(const char *filename, int hotx, int hoty);

#endif
