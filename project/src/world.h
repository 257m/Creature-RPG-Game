#ifndef WORLD_H
#define WORLD_H

#include <GL/gl.h>
#include <stdbool.h>
#include "data_types.h"

#define CHUNK_SIZE 16

typedef struct {
	u16 blk [CHUNK_SIZE][CHUNK_SIZE];	
} Chunk;

extern Chunk* chunk_generate(ivec2 pos);
extern void chunk_render(Chunk* c, vec2 offset);

typedef struct {
	Chunk*** chunks;
	u8 rend_dist;
	u32 chunks_size;
	ivec2 player_pos;
} World;

extern void world_init(World* w, u8 rend_dist);
extern void world_render(World* w);

#endif /* WORLD_H */
