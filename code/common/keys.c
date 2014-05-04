#include "keys.h"

KEY	key_decode(SDL_Keysym *k)
{
	if (k->sym >= KEY_SPACE && k->sym < KEY_DELETE) {
		return k->sym;
	}

	switch (k->sym) {
	case SDLK_DELETE:	return KEY_DELETE;
	case SDLK_CAPSLOCK:	return KEY_CAPSLOCK;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:	return KEY_SHIFT;
	case SDLK_LCTRL:
	case SDLK_RCTRL:	return KEY_CTRL;
	case SDLK_LALT:
	case SDLK_RALT:		return KEY_ALT;
	case SDLK_LGUI:
	case SDLK_RGUI:		return KEY_META;
	case SDLK_LEFT:		return KEY_LEFT;
	case SDLK_UP:		return KEY_UP;
	case SDLK_DOWN:		return KEY_DOWN;
	case SDLK_RIGHT:	return KEY_RIGHT;
	case SDLK_HOME:		return KEY_HOME;
	case SDLK_PAGEUP:	return KEY_PGUP;
	case SDLK_PAGEDOWN:	return KEY_PGDN;
	case SDLK_END:		return KEY_END;
	case SDLK_F1:		return KEY_F1;
	case SDLK_F2:		return KEY_F2;
	case SDLK_F3:		return KEY_F3;
	case SDLK_F4:		return KEY_F4;
	case SDLK_F5:		return KEY_F5;
	case SDLK_F6:		return KEY_F6;
	case SDLK_F7:		return KEY_F7;
	case SDLK_F8:		return KEY_F8;
	case SDLK_F9:		return KEY_F9;
	case SDLK_F10:		return KEY_F10;
	case SDLK_F11:		return KEY_F11;
	case SDLK_F12:		return KEY_F12;
	}

	return KEY_INVALID;
}
