#ifndef	_DFA_H_
#define	_DFA_H_

typedef enum dfa_states
{
	state_end,
	state_unknown
} dfa_states;

#define DFA_START_VERTEX 1
#define DFA_CORKSCREW_VERTEX 0
// 0 вершина это штопорное состояние, 1 - начальное
typedef struct dfa
{
	int count;
	struct dstate* adj_states;
} dfa;

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

/*_______________ другие функции _______________*/

dfa* dfa_get_inverse(dfa* d);

#endif // _DFA_H_