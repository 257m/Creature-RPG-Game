#ifndef STATE_H
#define STATE_H

#include <GLFW/glfw3.h>
#include "data_types.h"
#include "parties.h"

typedef struct {
	GLFWwindow* window;
	u32 window_width, window_height;

	Party* Parties [2];
} State;

enum {
	PLAYER,
	ENEMY,
};

extern State* state;

#endif /* STATE_H */