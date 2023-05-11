#ifndef IDEAMON_H
#define IDEAMON_H

#include "data_types.h"
#include "moves.h"

typedef struct {
	char name [12];
	u8 stats [6];
	u8 types [2];
} Ideamon_Dex;

typedef struct {
	Move_Slot moves [4];
	u8 evs [6];
	u16 dex_num;
	u8 ivs [6];
	u8 abilities [2];
	u8 traits [2];
	u8 nature;
	i32 current_hp;
	u8 level;
	u8 status;
	u8 item;
} Active_Ideamon; 

#endif /* IDEAMON_H */