#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <GLFW/glfw3.h>

typedef struct {
	int key, scancode, action, mods;
} Event;

typedef struct Event_Queue_Node {
	Event event;
	struct Event_Queue_Node* next;
} Event_Queue_Node;

typedef struct {
	Event_Queue_Node* first;
	Event_Queue_Node* last;
} Event_Queue;

extern void event_queue_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern Event* event_queue_poll(Event_Queue* event_queue, Event* event);
extern void event_queue_flush();

#endif /* EVENT_QUEUE_H */