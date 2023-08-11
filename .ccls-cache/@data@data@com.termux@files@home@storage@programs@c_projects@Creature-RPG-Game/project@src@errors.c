#include "state.h"

void clean_up_program()
{
	glfwDestroyWindow(state->window);
    glfwTerminate();
}