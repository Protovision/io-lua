#ifndef KEYS_H
#define KEYS_H

#include "SDL.h"

typedef enum {
	KEY_INVALID	= 0,
	KEY_BACKSPACE	= 8,
	KEY_TAB,
	KEY_RETURN	= 13,
	KEY_ESCAPE	= 27,
	KEY_SPACE	= 32,
	
	/* Normal keys match their ascii values here */

	KEY_DELETE	= 127,
	KEY_CAPSLOCK,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_ALT,
	KEY_META,
	KEY_LEFT,
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_HOME,
	KEY_PGUP,
	KEY_PGDN,
	KEY_END,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	
} KEY;

#endif
