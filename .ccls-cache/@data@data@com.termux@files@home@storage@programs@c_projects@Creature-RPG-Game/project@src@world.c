#include <malloc.h>
#include <stdbool.h>
#include "time.h"
#include "world.h"
#include "ui_elements.h"
#include "errors.h"
#include "texture.h"
#include "stb/stb_image.h"
#include "shader_utils.h"

static GLuint program;
static Texture tile_atlas;
GLuint chunk_vbo;
GLint uniform_offset;

Chunk* chunk_generate(ivec2 pos)
{
    Chunk* c = malloc(sizeof(Chunk));
    memset(c->blk, 0, sizeof(c->blk));
    return c;
}

vec4 chunk_vertices[CHUNK_SIZE * CHUNK_SIZE * 4];

void chunk_vertices_init()
{
    for (unsigned int i = 0; i < sizeof chunk_vertices / sizeof *chunk_vertices;) {
        vec2 offset = {(i % 16) - 1.0f, (i / 16)};

        chunk_vertices[i][0] = 0.0f + offset[0];
        chunk_vertices[i++][1] = 0.0f + offset[1];

        chunk_vertices[i][0] = 1.0f / 8.0f + offset[0];
        chunk_vertices[i++][1] = 0.0f + offset[1];

        chunk_vertices[i][0] = 1.0f / 8.0f + offset[0];
        chunk_vertices[i++][1] = 1.0f / 6.0f + offset[1];

        chunk_vertices[i][0] = 0.0f + offset[0];
        chunk_vertices[i++][1] = 1.0f / 6.0f + offset[1];
    }   
}

void chunk_render(Chunk* c, vec2 offset)
{
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
    glBindBuffer(GL_ARRAY_BUFFER, chunk_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof chunk_vertices, chunk_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glUniform2fv(uniform_offset, 2, offset);

    glDrawArrays(GL_QUADS, 0, CHUNK_SIZE*CHUNK_SIZE);
}


void world_init(World* w, u8 rend_dist)
{
    chunk_vertices_init();
    program = shader_create_program("src/shaders/world.v.glsl", "src/shaders/world.f.glsl");

    uniform_offset = glGetUniformLocation(program, "offset");

    glGenBuffers(1, &chunk_vbo);
	tile_atlas.data = stbi_load("images/tile_atlas.png", &tile_atlas.width, &tile_atlas.height, &tile_atlas.id, 4);
    glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tile_atlas.id);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_atlas.width, tile_atlas.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tile_atlas.data);
    stbi_image_free(tile_atlas.data);

    glUseProgram(program);
    glUniform2fv(uniform_offset, 2, &(vec2){0.0f, 0.0f});

    w->player_pos[0] = 0;
    w->player_pos[1] = 0;
}

void world_render(World* w)
{
    chunk_render(NULL, &(vec2){0.0f, 0.0f});
}
