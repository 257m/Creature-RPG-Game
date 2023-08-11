#include "event_queue.h"
#include "state.h"
#include "errors.h"

inline void* event_queue_memcpy(void* dest, const void* src, size_t len)
{
 	char *d = dest;
 	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

Event_Queue* event_queue_create(EVENT_SIZE_TYPE event_size)
{
	Event_Queue* event_queue = malloc(sizeof(Event_Queue));
	event_queue->events = malloc(event_size*sizeof(Event));
	PANIC(!event_queue->events, "Failed to allocate events array!\n");
	event_queue->head = 0;
	event_queue->tail = 0;
	event_queue->event_size = event_size;
	return event_queue;
}

void event_queue_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	#if EVENT_QUEUE_DYNAMIC
	if (state->event_queue->event_size < 0) {
		state->event_queue->events = realloc(state->event_queue->events, state->event_queue->event_size*-2*sizeof(Event));
		event_queue_memcpy(state->event_queue->events - state->event_queue->event_size, state->event_queue->events, state->event_queue->tail*sizeof(Event));
		state->event_queue->tail = state->event_queue->event_size;
		state->event_queue->event_size *= -2;
	}
	#endif
	state->event_queue->events[state->event_queue->tail] = (Event){key, scancode, action, mods};
	state->event_queue->tail = ++(state->event_queue->tail) % state->event_queue->event_size;
	#if EVENT_QUEUE_DYNAMIC
	if (state->event_queue->head == state->event_queue->tail)
		state->event_queue->event_size *= -1;
	#endif
}

Event* event_queue_poll(Event_Queue* event_queue, Event* event)
{
	#if EVENT_QUEUE_DYNAMIC
	if (event_queue->event_size < 0)
		event_queue->event_size *= -1;
	else 
	#endif
	if (event_queue->head == event_queue->tail)
		return NULL;
	*event = event_queue->events[event_queue->head];
	event_queue->head = ++(event_queue->head) % event_queue->event_size;
	return event;
}

inline void event_queue_flush(Event_Queue* event_queue)
{
	event_queue->head, event_queue->tail = 0;
}
