#include "state.h"
#include "texture.h"

void battle()
{
	while (!glfwWindowShouldClose(state->window)) {
		retrieve_user_move(PLAYER);
		retrieve_user_move(ENEMY);
	}
}