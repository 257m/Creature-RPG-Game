#include <time.h>
#include "cglm/mat4.h"
#include "cglm/types-struct.h"
#include "state.h"
#include "errors.h"
#include "texture.h"
#include "stb/stb_image.h"
#include "shader_utils.h"
#include "perlin.h"
#include "world.h"
#include <stdbool.h>
#include "cglm/cglm.h"

static GLuint program;
static Texture tile_atlas;
GLint attribute_coord;
GLint uniform_mvp;
u64 world_seed;

void update_vectors(f64 dx, f64 dy)
{
	state->world.forward[0] = sinf(state->world.angle[0]);
	state->world.forward[1] = 0;
	state->world.forward[2] = cosf(state->world.angle[0]);

	state->world.right[0] = -cosf(state->world.angle[0]);
	state->world.right[1] = 0;
	state->world.right[2] = sinf(state->world.angle[0]);

	state->world.lookat[0] = sinf(state->world.angle[0]) * cosf(state->world.angle[1]);
	state->world.lookat[1] = sinf(state->world.angle[1]);
	state->world.lookat[2] = cosf(state->world.angle[0]) * cosf(state->world.angle[1]);

	glm_cross(state->world.right, state->world.lookat, state->world.up);
}

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
	for (u32 i = 0; i < CHUNK_SIZE;) {
		c->blk[i++] = STONE;
		c->blk[i++] = GRASS;
	}
	c->elements = 0;
    c->changed = true;
    glGenBuffers(1, &c->vbo);
	return c;
}

inline void chunk_set(Chunk* c, u8 x, u8 y, u8 z, u16 s)
{
	c->blk[y*CHUNK_SIZE_X*CHUNK_SIZE_Z + x*CHUNK_SIZE_Z + z] = s;
	c->changed = true;
}

inline void chunk_set_raw(Chunk* c, u32 blk, u16 s)
{
	c->blk[blk] = s;
	c->changed = true;
}

void chunk_update(Chunk* c)
{
	c->changed = false;
	u8vec4s vertex[CHUNK_SIZE * 6 * 6];
	u32 i = 0;
	for (u8 y = 0; y < CHUNK_SIZE_Y; y++) {
		for (u8 x = 0; x < CHUNK_SIZE_X; x++) {
			for (u8 z = 0; z < CHUNK_SIZE_Z; z++) {
				u16 type = chunk_get(c, x, y, z);
				type &= 0xFF;
				if (!type)
					continue;
				type--;
				// Will refactor later
				// -x
				vertex[i++] = (u8vec4s){x,     y,     z,     type};        
        		vertex[i++] = (u8vec4s){x,     y,     z + 1, type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x,     y,     z + 1, type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z + 1, type};
				// +x
				vertex[i++] = (u8vec4s){x + 1, y,     z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z + 1, type};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z + 1, type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z + 1, type};
				// -y
				vertex[i++] = (u8vec4s){x,     y,     z,     type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z,     type + 128};        
        		vertex[i++] = (u8vec4s){x,     y,     z + 1, type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z,     type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z + 1, type + 128};        
        		vertex[i++] = (u8vec4s){x,     y,     z + 1, type + 128};
				// +y
				vertex[i++] = (u8vec4s){x,     y + 1, z,     type + 128};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z + 1, type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z,     type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z,     type + 128};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z + 1, type + 128};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z + 1, type + 128};
				// -z
				vertex[i++] = (u8vec4s){x,     y,     z,     type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z,     type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z,     type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z,     type};
				// +z
				vertex[i++] = (u8vec4s){x,     y,     z + 1, type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z + 1, type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z + 1, type};        
        		vertex[i++] = (u8vec4s){x,     y + 1, z + 1, type};        
        		vertex[i++] = (u8vec4s){x + 1, y,     z + 1, type};
        		vertex[i++] = (u8vec4s){x + 1, y + 1, z + 1, type};        
			}
		}
	}
	c->elements = i;
  	glBindBuffer(GL_ARRAY_BUFFER, c->vbo);
  	glBufferData(GL_ARRAY_BUFFER, c->elements * sizeof *vertex, vertex, GL_STATIC_DRAW);
}

void chunk_render(Chunk* c)
{
	/*glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);*/
	if (c->changed)
    	chunk_update(c);

  	if (!c->elements)
    	return;

  	glEnable(GL_CULL_FACE);
  	glEnable(GL_DEPTH_TEST);

  	glBindBuffer(GL_ARRAY_BUFFER, c->vbo);
  	glVertexAttribPointer(attribute_coord, 4, GL_BYTE, GL_FALSE, 0, 0);
  	glDrawArrays(GL_TRIANGLES, 0, c->elements);
}

void world_init(World* w, u8 rend_dist)
{
	world_seed = (rand() << 32) | rand();
	program = shader_create_program("src/shaders/world.v.glsl", "src/shaders/world.f.glsl");

	attribute_coord = glGetAttribLocation(program, "coord");
	uniform_mvp = glGetUniformLocation(program, "mvp");
	PANIC(attribute_coord == -1 || uniform_mvp == -1, "Could not get location of attribute coord and or uniform mvp!\r\n");

	tile_atlas.data = stbi_load("images/tile_atlas.png", &tile_atlas.width, &tile_atlas.height, &tile_atlas.id, 4);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tile_atlas.id);
	glBindTexture(GL_TEXTURE_2D, tile_atlas.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_atlas.width, tile_atlas.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tile_atlas.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(tile_atlas.data);

	glUseProgram(program);
	glClearColor(0.6, 0.8, 1.0, 0.0);
	glEnable(GL_CULL_FACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Use GL_NEAREST_MIPMAP_LINEAR if you want to use mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glPolygonOffset(1, 1);

	glEnableVertexAttribArray(attribute_coord);

	glm_vec3_copy(&(vec3s){0, CHUNK_SIZE_Y + 1, 0}, w->position);
	glm_vec3_copy(&(vec3s){0, -0.5f, 0}, w->angle);

	w->chunk = chunk_generate(&(vec2s){0, 0});
}

void world_set_center(World* w)
{
}

void world_render(World* w)
{
	world_set_center(w);
	mat4 view;
	vec3 center;
	glm_vec3_add(w->position, w->lookat, center);
	glm_lookat(w->position, center, w->up, view);
	mat4 projection;
	glm_perspective(45.0f, 768.0f/576.0f, 0.001f, 1000.0f, projection);

	mat4 mvp;
	glm_mat4_mul(projection, view, mvp);
	mat4 model = GLM_MAT4_IDENTITY_INIT;
	glm_translate(model, &(vec3s){0.0f, 0.0f, 0.0f});
	glm_mat4_mul(mvp, model, mvp);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, mvp);
	chunk_render(w->chunk);
}
