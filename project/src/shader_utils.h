#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GLFW/glfw3.h>

extern GLuint shader_compile(unsigned int type, const char* source_file);
extern GLuint shader_create_program(const char* vertex_file, const char* fragment_file);

#endif /* SHADER_UTILS_H */
