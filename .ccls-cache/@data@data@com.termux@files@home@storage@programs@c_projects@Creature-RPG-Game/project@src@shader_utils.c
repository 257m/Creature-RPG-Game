#include "shader_utils.h"
#include "errors.h"

GLuint shader_compile(unsigned int type, const char* source_file) {
    GLuint id = glCreateShader(type);
    FILE* sf = fopen(source_file, "rb");
	PANIC(!sf, "Could not open shader source file!\r\n");
    fseek(sf, 0, SEEK_END);
    unsigned int filelength = ftell(sf);
    fseek(sf, 0, SEEK_SET);
    const char* file = alloca(filelength);
    if (file)
        fread(file, 1, filelength, sf);
    fclose(sf);

	glShaderSource(id, 1, &file, NULL);
	glCompileShader(id);
	
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length);
		glGetShaderInfoLog(id, length, &length, message);
		fprintf(stderr, "Failed to compile %s shader\r\n",(type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
		glDeleteShader(id);
	}

	return id;
}

GLuint shader_create_program(const char* vertex_file, const char* fragment_file)
{
    GLuint program = glCreateProgram();
	PANIC(!program, "Error creating GL program!\n");
    unsigned int vs = shader_compile(GL_VERTEX_SHADER, vertex_file);
	unsigned int fs = shader_compile(GL_FRAGMENT_SHADER, fragment_file);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
