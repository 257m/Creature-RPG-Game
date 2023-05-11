#ifndef MOVES_H
#define MOVES_H

#include "data_types.h"

typedef struct {
	char name [16];
	u8 movefuncs [2];
	u8 bp;
	u8 accuracy;
	u8 pp;
	u8 type;
	i8 priority;
	u8 category;
} Move_Dex;

typedef struct {
	u16 move_num;
	i8 pp;
	i8 pp_mult;
} Move_Slot;

enum {
	NO_MOVE,
	STRUGGLE,
	SCRATCH,
	MOVE_MAX,
};

enum {
	MF_DEFAULT,
	MF_STRUGGLE,
	MF_MAX,
};

enum {
	STATUS  	=	0b00,
	PHYSICAL	=	0b01,
	MAGIC   	=	0b10,
	WEIRD   	=	0b11,
	CONTACT	=	0b0000,
	NEAR	=	0b0100,
	RANGED	=	0b1000,
	SPLASH	=	0b1100,
	SURFACE    	=	0b10000,
	AERIAL     	=	0b100000,
	UNDERGROUND =	0b1000000,
	UNDERWATER	=	0b10000000,
};

extern const Move_Dex move_data [MOVE_MAX];
typedef const void (*mf) (i8 time, u8 party, u8 slot);
extern mf move_funcs [MF_MAX];

#endif /* MOVES_H */