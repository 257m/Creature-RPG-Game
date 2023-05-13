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
	state->event_queue = alloca(sizeof(Event_Queue));
	state->event_queue->first = NULL;
	state->event_queue->last = NULL;
	srand(time(NULL));
	InitGLFW();
	battle();
	clean_up_program();
}