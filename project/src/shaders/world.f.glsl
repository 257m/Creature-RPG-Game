#version 330 core
varying vec2 texcoords;

uniform sampler2D tex;

void main(void)
{
    gl_FragColor = texture(tex, texcoords);
}
