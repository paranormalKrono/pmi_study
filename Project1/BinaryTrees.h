#ifndef	_BINARYTREES_H_
#define	_BINARYTREES_H_

typedef struct ubinary_tree {
	void* data;
	struct ubinary_tree* left;
	struct ubinary_tree* right;
} ubinary_tree;

ubinary_tree* utree_create(void** nums, int count);
ubinary_tree* utree_get_node(void* num);
void utree_free(ubinary_tree* tree);


typedef struct binary_tree {
	int data;
	struct binary_tree *left;
	struct binary_tree *right;
} binary_tree;

binary_tree* tree_create(int* nums, int count);
binary_tree* tree_get_node(int num);
void tree_show(const binary_tree* tree);
void tree_free(binary_tree* tree);


typedef struct binary_tree_SternBrocot {
	unsigned int p;
	unsigned int q;
	struct binary_tree_SternBrocot* left;
	struct binary_tree_SternBrocot* right;
} binary_tree_stern_brocot;

binary_tree_stern_brocot* tree_stern_brocot_create(unsigned int depth);
binary_tree_stern_brocot* tree_stern_brocot_node();
void tree_stern_brocot_show(const binary_tree_stern_brocot* tree);
char* get_stern_brocot_approximation(const binary_tree_stern_brocot* tree, double value);
void tree_stern_brocot_free(binary_tree_stern_brocot* tree);

#endif // _BINARYTREES_H_