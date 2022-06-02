#include "DFA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shunting_yard.h"
#include "Tokens.h"

dfa* dfa_implement(dfa* d)
{
	dfa* di = dfa_copy(d);
	for (int i = 0; i < di->count; ++i)
	{
		di->adj_states[i].state = d->adj_states[i].state == state_end ? state_unknown : state_end;
	}
	return di;
}

dfa* dfa_intersection(dfa* d1, dfa* d2)
{
	if (d1->count < 2 || d2->count < 2) return NULL;
	dfa* d = dfa_init(d1->count * d2->count);

    dnode *cur_node_d1, *cur_node_d2;
    dstate* s1 = d1->adj_states, *s2 = d2->adj_states;

    int c = d2->count, v1, v2;
    for (int i = 0; i < d1->count; ++i) 
    {
        for (int j = 0; j < c; ++j) 
        {
            d->adj_states[i * c + j].state = s1[i].state == state_end && s2[j].state == state_end ? state_end : state_unknown;

            cur_node_d1 = s1[i].head;
            cur_node_d2 = s2[j].head;
            while (cur_node_d1 && cur_node_d2) 
            {
                v1 = cur_node_d1->value;
                v2 = cur_node_d2->value;
                if (v1 == v2)
                {
                    dfa_add_arc(d, i * c + j, cur_node_d1->vertex * c + cur_node_d2->vertex, v1);
                    cur_node_d1 = cur_node_d1->next;
                    cur_node_d2 = cur_node_d2->next;
                }
                else if (v1 > v2) cur_node_d2 = cur_node_d2->next;
                else cur_node_d1 = cur_node_d1->next;
            }
            d->init_state = d1->init_state * c + d2->init_state;
        }
    }
    return d;
}

dfa* dfa_union(dfa* d1, dfa* d2)
{
    if (d1->count < 2 || d2->count < 2) return NULL;
    dfa* d = dfa_init(d1->count * d2->count);

    dnode* cur_node_d1, * cur_node_d2;
    dstate* s1 = d1->adj_states, * s2 = d2->adj_states;

    int c = d2->count, v1, v2;
    for (int i = 0; i < d1->count; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            d->adj_states[i * c + j].state = (s1[i].state == state_end || s2[j].state == state_end) ? state_end : state_unknown;

            cur_node_d1 = s1[i].head;
            cur_node_d2 = s2[j].head;
            while (cur_node_d1 && cur_node_d2)
            {
                v1 = cur_node_d1->value;
                v2 = cur_node_d2->value;
                if (v1 == v2)
                {
                    dfa_add_arc(d, i * c + j, cur_node_d1->vertex * c + cur_node_d2->vertex, v1);
                    //cur_node_d1 = cur_node_d1->next;
                    //cur_node_d2 = cur_node_d2->next;
                }
                //else if (v1 > v2) cur_node_d2 = cur_node_d2->next;
                //else cur_node_d1 = cur_node_d1->next;
                cur_node_d1 = cur_node_d1->next;
                cur_node_d2 = cur_node_d2->next;
            }
        }
    }
    return d;
}

dfa* dfa_difference(dfa* d1, dfa* d2) 
{
    dfa* d = dfa_intersection(d1, dfa_implement(d2));
    if (!d) return NULL;
    return d;
}

void show_queue2(queue* q)
{
    queue* c = queue_clone(q);
    token* e = token_alloc();
    while (queue_pop(c, &e))
    {
        printf("%s ", e->name);
    }
    printf("\n");
}

dfa* dfa_get_str_result(char* str, const vvariable* vars, int count) 
{
    queue* q = get_Reverse_Polish_Notation(str);
    show_queue2(q);
    dfa* d = (dfa*)get_RPN_result(q, vars, count);
    queue_free(q);
    return d;
}