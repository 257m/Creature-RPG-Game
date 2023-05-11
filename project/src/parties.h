#ifndef PARTIES_H
#define PARTIES_H

#include "ideamon.h"

#define PARTY_SIZE 6

typedef struct {
	Active_Ideamon** members;
	Move_Slot* turn;
} Party;

#endif /* PARTIES_H */