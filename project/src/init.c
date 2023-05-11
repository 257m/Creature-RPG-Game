#include "state.h"
#include "errors.h"

void InitGLFW()
{
	PANIC(!glfwInit(), "Failed to initialize GLFW\n");
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    state->window = glfwCreateWindow(state->window_width, state->window_height, "Pokemon Battle Simulator", NULL, NULL);
    
    PANIC(!state->window, "Failed to open GLFW window.\n");

    glfwMakeContextCurrent(state->window);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}