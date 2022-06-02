#ifndef	_DFA_H_
#define	_DFA_H_
#include "Variables_void.h"

typedef enum dfa_states
{
	state_end,
	state_unknown,
	state_init
} dfa_states;

#define DFA_START_VERTEX 1
#define DFA_CORKSCREW_VERTEX 0
// 0 вершина это штопорное состояние, 1 - начальное
typedef struct dfa
{
	int init_state;
	int count;
	struct dstate* adj_states;
} dfa;

// ноды в отсортированном по возрастанию значения порядке
typedef struct dstate
{
	dfa_states state;
	struct dnode* head;
} dstate;

typedef struct dnode
{
	int value;
	int vertex;
	struct dnode* next;
} dnode;


//dfa* dfa_alloc();
dfa* dfa_init(int states_count);
dfa* dfa_copy(dfa* d);
void dfa_free(dfa* d);

// value из first в second и !value в штопорное
void dfa_add_arcs(dfa* d, int first, int second, int value);

void dfa_add_arc(dfa* d, int first, int second, int value);
void dfa_del_arc(dfa* d, int first, int second, int value);

void dfa_print(dfa* g);

dnode* dnode_init(int value, int vertex, dnode* next);
void dnode_free(dnode* n);

// Множество из 1 элемента
dfa* dfa_int(int number); 
int dfa_check_int(dfa* d, const int n); // узнать, распознаёт ли автомат число

// вывести числа в диапазоне [0, number]
void dfa_print_range(dfa* d, int number); 
void dfa_print_range2(dfa* d, int number);

/*_______________ другие функции _______________*/

dfa* dfa_implement(dfa* d);
dfa* dfa_intersection(dfa* d1, dfa* d2);
dfa* dfa_union(dfa* d1, dfa* d2);
dfa* dfa_difference(dfa* d1, dfa* d2);

dfa* dfa_get_str_result(char* str, const vvariable* vars, int count);

#endif // _DFA_H_