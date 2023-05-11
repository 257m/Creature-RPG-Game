#include "moves.h"
#include "types.h"

const Move_Dex move_data [MOVE_MAX] = {
	{
		.name = "NULL",
	},
	{
		.name = "Struggle",
		.bp = 40,
		.accuracy = 101,
		.pp = 0,
		.type = NO_TYPE,
		.priority = 0,
		.category = WEIRD | CONTACT | SURFACE | UNDERWATER | UNDERGROUND | AERIAL,
	},
	{
		.name = "Scratch",
		.bp = 40,
		.accuracy = 100,
		.pp = 2,
		.type = NORMAL,
		.priority = 0,
		.category = PHYSICAL | CONTACT | SURFACE | UNDERWATER,
	},
};

void mf_default(i8 time, u8 party, u8 slot)
{
	/* Dummy Func */
	return;
}

void mf_struggle(i8 time, u8 party, u8 slot)
{
	/* Will deal with this later */
	return;
}

 mf move_funcs [MF_MAX] = {
	&mf_default,
	&mf_struggle,
};