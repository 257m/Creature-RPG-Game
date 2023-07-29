#ifndef STATE_H
#define STATE_H

#include <GLFW/glfw3.h>
#include "data_types.h"
#include "event_queue.h"
#include "world.h"

typedef struct {
	GLFWwindow* window;
	u32 window_width, window_height;
	Event_Queue* event_queue;
	World world;
} State;

extern State* state;

#endif /* STATE_H */
