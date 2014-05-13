#ifndef COMMON_H
#define COMMON_H

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifndef _WINDOWS
#include <unistd.h>
#include <sys/stat.h>
#define MKDIR(P,M)	mkdir((P),(M))
#define RMDIR(P)	rmdir((P))
#define GETCWD(B,L)	getcwd((B),(L))
#else
#include <direct.h>
#define MKDIR(P, M)	_mkdir((P))
#define RMDIR(P)	_rmdir((P))
#define GETCWD(B,L)	_getcwd((B),(L))
#endif

#define	ENABLE_EVENT_MOUSEMOVE	0

#define MAX_PATH		256
#define MAX_STRING		32000

#define ERROR(F, ...)	do { \
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
	FILE_SET,
	FILE_CUR,
	FILE_END
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

void	script_init(int);
void	script_shutdown();
void	script_load(const char *luafile);
void	script_register(const char *cmdname, int (*func)(lua_State*));
void	script_call(const char *func, const char *fmt, ...);

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

void	mem_init(int, int, int, int);
void	mem_shutdown();
void	*mem_alloc(size_t size);
void	*mem_realloc(void *ptr, size_t size);
void	mem_free(void *ptr);
char	*mem_strdup(const char *s);


#define	FONT	TTF_Font

#define		font_init()		(TTF_Init())
#define		font_shutdown()		(TTF_Quit())
#define		font_free(F)		(TTF_CloseFont((F)))
#define		font_load(F,S)		(TTF_OpenFont((F),(S)))

/* =====================================================
 * image.c
 * =====================================================
 */

#define IMAGE	SDL_Texture

#define		image_init()		(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG))
#define		image_shutdown()	(IMG_Quit())
#define		image_free(I)		(SDL_DestroyTexture((I)))

SDL_Texture	*image_load(const char *file, int w, int h);
void		image_colorize(SDL_Texture *img, Uint32 hue);

/* =====================================================
 * util.c
 * =====================================================
 */

char	*va(const char *fmt, ...);
char	*gamepath(const char *path);
char	*basepath(const char *path);

void	sys_copy(const char *from, const char *to);
int	sys_exists(const char *path);

#endif
