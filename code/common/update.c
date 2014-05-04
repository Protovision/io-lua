#include "common.h"
#include "video/video.h"

var_t	*c_fps;

void	update()
{
	event_t ev;

	event_pull(&ev);
	
	switch (ev.type) {
	case EVENT_KEYBOARD:
		script_call("main", "iii", EVENT_KEYBOARD,
			ev.key.key, ev.key.pressed);
		break;
	case EVENT_MOUSEBUTTON:
		script_call("main", "iiiii", EVENT_MOUSEBUTTON,
			ev.button.x, ev.button.y, ev.button.button, ev.button.pressed);	
		break;
	case EVENT_MOUSEWHEEL:
		script_call("main", "ii", EVENT_MOUSEWHEEL, ev.scroll.dir);
		break;

#if ENABLE_EVENT_MOUSEMOVE == 1
	case EVENT_MOUSEMOVE:
		script_call("main", "iii", EVENT_MOUSEMOVE, ev.move.x, ev.move.y);
		break;
#endif
	default:
		break;
	}

	script_call("main", "i", EVENT_UPDATE);
	video_render();
}
