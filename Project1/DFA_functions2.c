#include "DFA.h"
#include <stdio.h>
#include <stdlib.h>

dfa* dfa_get_inverse(dfa* d)
{
	dfa* di = dfa_copy(d);
	for (int i = 0; i < di->count; ++i)
	{
		di->adj_states[i].state = di->adj_states[i].state == state_end ? state_unknown : state_end;
	}
	return di;
}

dfa* dfa_intersection(dfa* d1, dfa* d2)
{
	if (d1->count < 2 || d2->count < 2) return NULL;
	dnode* cur_d1_node, *cur_d2_node;
	dfa* d = dfa_init(d1->count < d2->count ? d1->count : d2->count);

	cur_d1_node = d1->adj_states[DFA_START_VERTEX].head;
	cur_d2_node = d2->adj_states[DFA_START_VERTEX].head;

	
}

dfa* dfa_union(dfa* d1, dfa* d2)
{
	dnode* cur_d1_node, cur_d2_node;
	dfa* d = dfa_init(d1->count * d2->count);


}