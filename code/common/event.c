#include "common.h"

#define	EVENT_MAX	256
static event_t		events[EVENT_MAX];
static event_t		*event_first;
static event_t		*event_last;

void	event_init()
{
	event_first = events;
	event_last = events;
}

void	event_shutdown()
{

}

void	event_push(event_t *ev)
{
	if (event_last == (events+EVENT_MAX) ) {
		event_last = events;
	}

	*event_last++ = *ev;
}

void	event_pull(event_t *ev)
{
	if (event_first == event_last) {
		ev->type = EVENT_NONE;
		return;
	}

	if (event_first == (events+EVENT_MAX) ) {
		event_first = events;
	}
	
	*ev = *event_first++;
}
