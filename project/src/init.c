#include "state.h"
#include "errors.h"

void InitGLFW()
{
	PANIC(!glfwInit(), "Failed to initialize GLFW\n");
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    state->window = glfwCreateWindow(state->window_width, state->window_height, "Creature RPG Game", NULL, NULL);
    
    PANIC(!state->window, "Failed to open GLFW window.\n");

    glfwMakeContextCurrent(state->window);
	glfwSetKeyCallback(state->window, event_queue_key_callback);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
}
