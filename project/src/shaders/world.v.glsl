#version 330 core
layout(location = 0) in vec2 coord;
layout(location = 1) in vec2 atexcoords;

uniform vec2 offset;
varying vec2 texcoords;

void main(void) {
	gl_Position = vec4(coord + offset, 0.0, 1.0);
	texcoords = atexcoords;
}
