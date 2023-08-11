#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <GLFW/glfw3.h>
#include "data_types.h"

#define EVENT_QUEUE_DYNAMIC 0
#if EVENT_QUEUE_DYNAMIC
	#define EVENT_SIZE_TYPE u64
#else
	#define EVENT_SIZE_TYPE i64
#endif

typedef struct {
	int key, scancode, action, mods;
} Event;

typedef struct {
	Event* events;
	u64 head, tail;
	EVENT_SIZE_TYPE event_size;
} Event_Queue;

extern Event_Queue* event_queue_create(EVENT_SIZE_TYPE event_size);
extern void event_queue_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern Event* event_queue_poll(Event_Queue* event_queue, Event* event);
extern inline void event_queue_flush();

#endif /* EVENT_QUEUE_H */
