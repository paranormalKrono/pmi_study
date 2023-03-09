#ifndef	_FIBONACCI_TREE_H_
#define	_FIBONACCI_TREE_H_
#include <stdbool.h>

typedef struct fibonacci_heap 
{
	int n_count;
	struct node_f* max;
	struct node_f* root;
} fibonacci_heap;

typedef struct node_f 
{
	void* data;

	float key;
	int degree;
	bool mark;

	struct node_f* parent;
	struct node_f* child;
	struct node_f* left;
	struct node_f* right;
} node_f;

void print_fibonacci_heap_node(node_f* cur_node);
void print_fibonacci_heap(fibonacci_heap* fh);

node_f* node_f_init(float key, void* data);
void fibonacci_heap_insert_node(fibonacci_heap* fh, node_f* node);
fibonacci_heap* fibonacci_heap_init();
node_f* fibonacci_heap_insert(fibonacci_heap* fh, float key, void* data);
fibonacci_heap* fibonacci_heap_union(fibonacci_heap* fh1, fibonacci_heap* fh2); 
node_f* fibonacci_heap_extract_max(fibonacci_heap* fh);
void fibonacci_heap_increase_key(fibonacci_heap* fh, node_f* x, float new_value);
node_f* fibonacci_heap_delete_key(fibonacci_heap* fh, node_f* x);

#endif // _FIBONACCI_TREE_H_