#include <stdio.h>
#include "state.h"
#include "errors.h"
#include "init.h"
#include "battle.h"

State* state;

int main()
{
	state = alloca(sizeof(State));
	state->window_width = 768;
	state->window_height = 576;
	srand(time(NULL));
	InitGLFW();
	battle();
	clean_up_program();
}