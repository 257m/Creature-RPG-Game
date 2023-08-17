#ifndef WORLD_H
#define WORLD_H

#include <GL/gl.h>
#include <stdbool.h>
#include "data_types.h"

#define CHUNK_SIZE 16

enum {
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

typedef struct Chunk_t {
	u16 blk [CHUNK_SIZE * CHUNK_SIZE];	
	struct Chunk_t* neighbors [4];
} Chunk;

extern Chunk* chunk_generate(ivec2 pos);
extern void chunk_render(Chunk* c, vec2 offset);

typedef struct {
	// World has access to chunk at 0,0 and chunk that the player is on
	Chunk* chunks;
	Chunk* current_chunk;
	ivec2 chunk_pos;
	u8 rend_dist;
	ivec2 player_pos;
} World;

extern void world_init(World* w, u8 rend_dist);
extern void world_render(World* w);

enum {
	GRASS,
	DIRT,
	STONE,
	WATER,
};

#endif /* WORLD_H */
