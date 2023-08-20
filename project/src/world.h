#ifndef WORLD_H
#define WORLD_H

#include <GL/gl.h>
#include <stdbool.h>
#include "data_types.h"
#include "cglm/vec3.h"

#define CHUNK_SIZE_H 16
#define CHUNK_SIZE_V 16
#define CHUNK_SIZE_X CHUNK_SIZE_H
#define CHUNK_SIZE_Y CHUNK_SIZE_V
#define CHUNK_SIZE_Z CHUNK_SIZE_H
#define CHUNK_SIZE (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

typedef struct Chunk_t {
	u16 blk [CHUNK_SIZE];	
	GLuint vbo;
	int elements;
	bool changed;
} Chunk;

extern void update_vectors(f64 dx, f64 dy);

extern Chunk* chunk_generate(ivec2 pos);
extern void chunk_update(Chunk* c);
extern void chunk_render(Chunk* c);
extern inline void chunk_set(Chunk* c, u8 x, u8 y, u8 z, u16 s);
extern inline void chunk_set_raw(Chunk* c, u32 blk, u16 s);
#define chunk_get(c, x, y, z) c->blk[y*CHUNK_SIZE_X*CHUNK_SIZE_Z + x*CHUNK_SIZE_Z + z];

typedef struct {
	Chunk* chunk;
	vec3 position;
	vec3 forward;
    vec3 right;
    vec3 up;
    vec3 lookat;
    vec3 angle;
	u8 rend_dist;
} World;

extern void world_init(World* w, u8 rend_dist);
extern void world_render(World* w);

enum {
	AIR,
	GRASS,
	DIRT,
	STONE,
	WATER,
};

#endif /* WORLD_H */
