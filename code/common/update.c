#include "common.h"
#include "video/video.h"

var_t	*c_fps;

void	update()
{
	event_t ev;
	
	event_pull(&ev);
	
	switch (ev.type) {
	case EVENT_KEYBOARD:
		script_call("keyboard", "ii",
			ev.key.key, ev.key.pressed);
		break;
	case EVENT_MOUSEBUTTON:
		script_call("mousebutton", "iiii", 
			ev.button.x, ev.button.y, ev.button.button, ev.button.pressed);	
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
	default:
		break;
	}

	script_call("update", NULL);
	video_render();
}
