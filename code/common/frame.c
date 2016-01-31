#include "common.h"
#include "video.h"

static void input()
{
	SDL_Event e;
	event_t ev;

	while ( SDL_PollEvent(&e) ) {

		switch ( e.type ) {

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ev.type = EVENT_KEYBOARD;
			ev.key.pressed = (e.type == SDL_KEYDOWN);
			ev.key.key = key_decode(&e.key.keysym);
			event_push(&ev);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			ev.type = EVENT_MOUSEBUTTON;
			ev.button.pressed = (e.type == SDL_MOUSEBUTTONDOWN);
			switch (e.button.button) {
			case SDL_BUTTON_LEFT: ev.button.button = BUTTON_LEFT; break;
			case SDL_BUTTON_RIGHT: ev.button.button = BUTTON_RIGHT; break;
			case SDL_BUTTON_MIDDLE: ev.button.button = BUTTON_MIDDLE; break;
			}
			ev.button.x = e.button.x;
			ev.button.y = e.button.y;
			event_push(&ev);
			break;
#if ENABLE_EVENT_MOUSEWHEEL == 1
		case SDL_MOUSEWHEEL:
			ev.type = EVENT_MOUSEWHEEL;
			ev.scroll.dir = (e.wheel.y ? SCROLL_UP : SCROLL_DOWN);
			event_push(&ev);
			break;
#endif

#if ENABLE_EVENT_MOUSEMOVE == 1
		case SDL_MOUSEMOTION:
			ev.type = EVENT_MOUSEMOVE;
			ev.move.x = e.motion.xrel;
			ev.move.y = e.motion.yrel;
			event_push(&ev);
			break;			
#endif

		case SDL_QUIT:
			quit(0);
			break;

		}

	}	
}

static void	update()
{
	event_t ev;

	event_pull(&ev);
	
	switch (ev.type) {
	case EVENT_KEYBOARD:
		script_call("keyboard", "ii", ev.key.key, ev.key.pressed);
		break;
	case EVENT_MOUSEBUTTON:
		script_call("mousebutton", "iiii", ev.button.x, ev.button.y, ev.button.button, ev.button.pressed);
		break;
#if ENABLE_EVENT_MOUSEWHEEL == 1
	case EVENT_MOUSEWHEEL:
		script_call("mousewheel", "i", ev.scroll.dir);
		break;
#endif

#if ENABLE_EVENT_MOUSEMOVE == 1
	case EVENT_MOUSEMOVE:
		script_call("mousemove", "ii", ev.move.x, ev.move.y);
		break;
#endif
	case EVENT_NONE:
	default:
		break;
	}

	script_call("update", NULL);
	video_render();	
}

void	frame()
{
	Uint32 start, elapsed, delay;

	input();

	start = SDL_GetTicks();

	update();

	delay = 1000/c_fps->integer;
	elapsed = SDL_GetTicks() - start;

	if (elapsed < delay) {
		SDL_Delay(delay - elapsed);
	}
}	
