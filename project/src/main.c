#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <alloca.h>
#include "cglm/types-struct.h"
#include "state.h"
#include "errors.h"
#include "init.h"

State* state;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static const float mousespeed = 0.0001f;

	state->xpos = xpos;
	state->ypos = ypos;

	state->world.angle[0] -= (xpos - state->window_width / 2) * mousespeed;
	state->world.angle[1] -= (ypos - state->window_height / 2) * mousespeed;
	if(state->world.angle[0] < -M_PI)
		state->world.angle[0] += M_PI * 2;
	if(state->world.angle[0] > M_PI)
		state->world.angle[0] -= M_PI * 2;
	if(state->world.angle[1] < -M_PI / 2)
		state->world.angle[1] = -M_PI / 2;
	if(state->world.angle[1] > M_PI / 2)
		state->world.angle[1] = M_PI / 2;

	update_vectors(xpos, ypos);
	glfwSetCursorPos(state->window, state->window_width / 2, state->window_height / 2);
}

int main()
{
	srand(time(NULL));
	state = alloca(sizeof(State));
	state->window_width = 768;
	state->window_height = 576;
	state->xpos = 0;
	state->ypos = 0;
	state->event_queue = event_queue_create(1024);
	InitGLFW();
	glfwSetCursorPos(state->window, state->window_width / 2, state->window_height / 2);
	world_init(&state->world, 1);
	Event e;
	double last_time = 0;
	double current_time = 0;
	while (!glfwWindowShouldClose(state->window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		current_time = glfwGetTime();
		double dt = current_time - last_time;
		last_time = current_time;
		const double movespeed = 10;
		while (event_queue_poll(state->event_queue, &e)) {
			if (e.action == GLFW_PRESS)
				switch(e.key) {
					case GLFW_KEY_ESCAPE:
						glfwSetWindowShouldClose(state->window, GL_TRUE);
						break;
				}
			if (e.action == GLFW_REPEAT) {
				switch (e.key) {
					case GLFW_KEY_W:
					case GLFW_KEY_UP:
						glm_vec3_muladds(state->world.forward, movespeed * dt, state->world.position);
						break;
					case GLFW_KEY_S:
					case GLFW_KEY_DOWN:
						glm_vec3_muladds(state->world.forward, -movespeed * dt, state->world.position);
						break;
					case GLFW_KEY_D:
					case GLFW_KEY_RIGHT:
						glm_vec3_muladds(state->world.right, movespeed * dt, state->world.position);
						break;
					case GLFW_KEY_A:
					case GLFW_KEY_LEFT:
						glm_vec3_muladds(state->world.right, -movespeed * dt, state->world.position);
						break;
					case GLFW_KEY_PAGE_DOWN:
						state->world.position[1] -= movespeed * dt;
						break;
					case GLFW_KEY_PAGE_UP:
						state->world.position[1] += movespeed * dt;
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
