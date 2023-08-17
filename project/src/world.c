#include "time.h"
#include "world.h"
#include "ui_elements.h"
#include "errors.h"
#include "texture.h"
#include "stb/stb_image.h"
#include "shader_utils.h"
#include "perlin.h"

static GLuint program;
static Texture tile_atlas;
GLuint chunk_vbo;
GLuint chunk_tbo;
GLint uniform_offset;
u64 world_seed;

u16 height_to_block_map(f32 h)
{
	if (h < -0.8)
		return WATER;
	if (h < 0)
		return STONE;
	if (h < 0.2)
		return DIRT;
	return GRASS;
}

#define PERLIN_ZOOM 4.0f
#define PERLIN_ZOOM_BLOCK_SIZE (16.0f / PERLIN_ZOOM)

Chunk* chunk_generate(ivec2 pos)
{
	Chunk* c = malloc(sizeof(Chunk));
	vec2 perlin_pos = {pos[0] * PERLIN_ZOOM, pos[1] * PERLIN_ZOOM};
	// Grab perlin map data and set the blocks to it
	for (unsigned int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
		c->blk[i] = height_to_block_map(perlin(world_seed, pos[0] + (i % 16) / PERLIN_ZOOM_BLOCK_SIZE, pos[1] + (i / 16) / PERLIN_ZOOM_BLOCK_SIZE));
	// Set all neighbors to null
	memset(&c->neighbors, 0, sizeof(c->neighbors));
	return c;
}

vec2 chunk_vertices[CHUNK_SIZE * CHUNK_SIZE * 4];
vec2 chunk_texcoords[CHUNK_SIZE * CHUNK_SIZE * 4];

void chunk_vertices_init()
{
	for (unsigned int i = 0; i < sizeof(chunk_vertices) / sizeof(*chunk_vertices);) {
		unsigned int block = i / 4;
		vec2 offset = {((block % 16) / 8.0f) - 1.0f, ((block / 16) / 6.0f) - 1.0f};

		chunk_texcoords[i][0] = 0.0f;
		chunk_texcoords[i][1] = 1.0f / 16.0f;
		chunk_vertices[i][0] = 0.0f + offset[0];
		chunk_vertices[i++][1] = 0.0f + offset[1];

		chunk_texcoords[i][0] = 1.0f / 16.0f;
		chunk_texcoords[i][1] = 1.0f / 16.0f;
		chunk_vertices[i][0] = (1.0f / 8.0f) + offset[0];
		chunk_vertices[i++][1] = 0.0f + offset[1];

		chunk_texcoords[i][0] = 1.0f / 16.0f;
		chunk_texcoords[i][1] = 0.0f;
		chunk_vertices[i][0] = (1.0f / 8.0f) + offset[0];
		chunk_vertices[i++][1] = (1.0f / 6.0f) + offset[1];

		chunk_texcoords[i][0] = 0.0f;
		chunk_texcoords[i][1] = 0.0f;
		chunk_vertices[i][0] = 0.0f + offset[0];
		chunk_vertices[i++][1] = (1.0f / 6.0f) + offset[1];
	}   
}

void chunk_texcoords_set(Chunk* c)
{
	for (unsigned int i = 0; i < sizeof(chunk_texcoords) / sizeof(*chunk_texcoords);) {
		unsigned int block = i / 4;
		vec2 offset = {(c->blk[block] % 16) / 16.0f, (c->blk[block] / 16) / 16.0f};

		chunk_texcoords[i][0] = 0.0f + offset[0];
		chunk_texcoords[i++][1] = 1.0f / 16.0f + offset[1];

		chunk_texcoords[i][0] = 1.0f / 16.0f + offset[0];
		chunk_texcoords[i++][1] = 1.0f / 16.0f + offset[1];

		chunk_texcoords[i][0] = 1.0f / 16.0f + offset[0];
		chunk_texcoords[i++][1] = 0.0f + offset[1];

		chunk_texcoords[i][0] = 0.0f + offset[0];
		chunk_texcoords[i++][1] = 0.0f + offset[1];
	}
}

void chunk_render(Chunk* c, vec2 offset)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	chunk_texcoords_set(c);
	glBindBuffer(GL_ARRAY_BUFFER, chunk_tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof chunk_texcoords, chunk_texcoords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*chunk_texcoords), (void*)0);
	glEnableVertexAttribArray(1);
	glUniform2fv(uniform_offset, 1, offset);

	glDrawArrays(GL_QUADS, 0, CHUNK_SIZE * CHUNK_SIZE * 4);
}


void world_init(World* w, u8 rend_dist)
{
	world_seed = (rand() << 32) | rand();
	chunk_vertices_init();
	program = shader_create_program("src/shaders/world.v.glsl", "src/shaders/world.f.glsl");

	uniform_offset = glGetUniformLocation(program, "offset");
	PANIC(uniform_offset == -1, "Could not get location of uniform offset!\r\n");

	glGenBuffers(1, &chunk_vbo);
	glGenBuffers(1, &chunk_tbo);
	tile_atlas.data = stbi_load("images/tile_atlas.png", &tile_atlas.width, &tile_atlas.height, &tile_atlas.id, 4);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tile_atlas.id);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_atlas.width, tile_atlas.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tile_atlas.data);
	stbi_image_free(tile_atlas.data);

	glUseProgram(program);

	w->player_pos[0] = 0;
	w->player_pos[1] = 0;

	glBindBuffer(GL_ARRAY_BUFFER, chunk_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof chunk_vertices, chunk_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(*chunk_vertices), (void*)0);
	glEnableVertexAttribArray(0);

	w->chunks = chunk_generate(&(vec2){0, 0});
	w->current_chunk = w->chunks;
	w->chunk_pos[0] = 0;
	w->chunk_pos[1] = 0;
}

void world_set_center(World* w)
{
	// If we already at the center return back to caller
	if (w->chunk_pos[0] == (w->player_pos[0] / CHUNK_SIZE) &&
		w->chunk_pos[1] == (w->player_pos[1] / CHUNK_SIZE))
		return;
	ivec2 desired_center = {w->player_pos[0] / CHUNK_SIZE, w->player_pos[1] / CHUNK_SIZE};
	ivec2 offset_vector = {w->chunk_pos[0] - desired_center[0], w->chunk_pos[1] - desired_center[1]};
	// Solution 0: Composition
	/*
	if (offset_vector[0] > 0)
		while (w->chunk_pos[0]-- != desired_center[0])
			if (w->current_chunk->neighbors[RIGHT])
				w->current_chunk = w->current_chunk->neighbors[RIGHT];
			else
				w->current_chunk = (w->current_chunk->neighbors[RIGHT]
						= chunk_generate(w->chunk_pos));
	else if (offset_vector[0] < 0)
		while (w->chunk_pos[0]++ != desired_center[0])
			if (w->current_chunk->neighbors[LEFT])
				w->current_chunk = w->current_chunk->neighbors[LEFT];
			else
				w->current_chunk = (w->current_chunk->neighbors[LEFT]
						= chunk_generate(w->chunk_pos));
	if (offset_vector[1] > 0)
		while (w->chunk_pos[1]-- != desired_center[1])
			if (w->current_chunk->neighbors[UP])
				w->current_chunk = w->current_chunk->neighbors[UP];
			else
				w->current_chunk = (w->current_chunk->neighbors[UP]
						= chunk_generate(w->chunk_pos));
	else if (offset_vector[1] < 0)
		while (w->chunk_pos[0]++ != desired_center[0])
			if (w->current_chunk->neighbors[DOWN])
				w->current_chunk = w->current_chunk->neighbors[DOWN];
			else
				w->current_chunk = (w->current_chunk->neighbors[DOWN]
						= chunk_generate(w->chunk_pos));
	w->chunk_pos[0] = desired_center[0];
	w->chunk_pos[1] = desired_center[1];
	*/
	// Solution 1: Loop
	for (u8 i = 0; i < 2; i++) {
		u8 dir = i*2 + (offset_vector[i] > 0);
		while (w->chunk_pos[i] != desired_center[i]) {
			w->chunk_pos[i] += (offset_vector[i] > 0)*2 - 1;
			if (w->current_chunk->neighbors[dir])
				w->current_chunk = w->current_chunk->neighbors[dir];
			else
				w->current_chunk = (w->current_chunk->neighbors[dir]
						= chunk_generate(w->chunk_pos));
		}
	}
}

void world_render(World* w)
{
	world_set_center(w);
	vec2 offset = {0.0f - ((f32)w->player_pos[0] / 8.0f), 0.0f - ((f32)w->player_pos[1] / 6.0f)};
}
