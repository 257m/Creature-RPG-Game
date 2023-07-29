#include <stdio.h>
#include "state.h"
#include "errors.h"
#include "init.h"

State* state;

int main()
{
	srand(time(NULL));
	state = alloca(sizeof(State));
	state->window_width = 768;
	state->window_height = 576;
	state->event_queue = event_queue_create(1024);
	InitGLFW();
	world_init(&state->world, 1);
	Event e;
	while (!glfwWindowShouldClose(state->window)) {
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		while (event_queue_poll(state->event_queue, &e)) {
			if (e.action == GLFW_PRESS) {
				switch (e.key) {
					case GLFW_KEY_ESCAPE:
						glfwSetWindowShouldClose(state->window, GL_TRUE);
						break;
					case GLFW_KEY_W:
					case GLFW_KEY_UP:
						state->world.player_pos[1]--;
						break;
					case GLFW_KEY_S:
					case GLFW_KEY_DOWN:
						state->world.player_pos[1]++;
						break;
					case GLFW_KEY_D:
					case GLFW_KEY_RIGHT:
						state->world.player_pos[0]++;
						break;
					case GLFW_KEY_A:
					case GLFW_KEY_LEFT:
						state->world.player_pos[0]--;
						break;
					default:
						break;
				}
			}
		}
		world_render(&state->world);
		glfwSwapBuffers(state->window);
		glfwPollEvents();
	}
	clean_up_program();
}
