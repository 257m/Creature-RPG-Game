#include "event_queue.h"
#include "state.h"

void event_queue_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Event_Queue_Node* next = malloc(sizeof(Event_Queue_Node));
	next->event.key = key;
	next->event.scancode = scancode;
	next->event.action = action;
	next->event.mods = mods;
	next->next = NULL;
	if (!state->event_queue->first) {
		state->event_queue->first = next;
		state->event_queue->last = next;
		return;
	}
	state->event_queue->last->next = next;
	state->event_queue->last = next;
}

Event* event_queue_poll(Event_Queue* event_queue, Event* event)
{
	if (!event_queue->first)
		return NULL;
	Event_Queue_Node* first = event_queue->first;
	event_queue->first = event_queue->first->next;
	*event = first->event;
	free(first);
	return event;
}

void event_queue_flush(Event_Queue* event_queue)
{
	while (event_queue->last = event_queue->first) {
		event_queue->first = event_queue->first->next;
		free(event_queue->last);
	}
	event_queue->last = NULL;
}