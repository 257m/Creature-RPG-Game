#include "state.h"
#include "parties.h"

void party_create(Party* p)
{
	p->members = malloc(sizeof(typeof(*p->members)*) * PARTY_SIZE);
	typeof(*p->members)* last = p->members + PARTY_SIZE;
	while (p->members < last)
		*p->members++ = malloc(sizeof(typeof(*p->members)));
}

void parties_create()
{
	state->Parties[0] = malloc(sizeof(Party));
	state->Parties[1] = malloc(sizeof(Party));

	party_create(state->Parties[0]);
	party_create(state->Parties[1]);
}