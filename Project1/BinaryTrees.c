#include "BinaryTrees.h"
#include <string.h>


void tree_add_subtree(binary_tree* tree, int num);

binary_tree* tree_create(int *nums, int count) 
{
	binary_tree *tree = tree_get_node(nums[0]);
	binary_tree* currentLeaf = tree;
	
	for(int i = 1; i < count; ++i)
		tree_add_subtree(tree, nums[i]);
	return tree;
}

void tree_add_subtree(binary_tree* tree, int num)
{
	if (num < tree->data)
	{
		if (tree->left == NULL)
			tree->left = tree_get_node(num);
		else
			tree_add_subtree(tree->left, num);
	}
	else
	{
		if (tree->right == NULL)
			tree->right = tree_get_node(num);
		else
			tree_add_subtree(tree->right, num);
	}
}

void tree_stern_brocot_fill(binary_tree_stern_brocot *current,
	binary_tree_stern_brocot *left,
	binary_tree_stern_brocot *right,
	unsigned int cur_depth,
	unsigned int depth) 
{
	if (cur_depth < depth)
	{
		++cur_depth;
		current->left = tree_stern_brocot_node(left->p + current->p, left->q + current->q);
		current->right = tree_stern_brocot_node(right->p + current->p, right->q + current->q);
		tree_stern_brocot_fill(current->left, left, current, cur_depth, depth);
		tree_stern_brocot_fill(current->right, current, right, cur_depth, depth);
	}
}

binary_tree_stern_brocot* tree_stern_brocot_create(unsigned int depth) 
{
	binary_tree_stern_brocot* tree = NULL;
	if (depth)
	{
		tree = tree_stern_brocot_node(1, 1);

		binary_tree_stern_brocot* left = tree_stern_brocot_node(0, 1);
		binary_tree_stern_brocot* right = tree_stern_brocot_node(1, 0);

		tree_stern_brocot_fill(tree, left, right, 0, depth);

		tree_stern_brocot_free(left);
		tree_stern_brocot_free(right);
	}
	return tree;
}

binary_tree* tree_get_node(int num) 
{
	binary_tree* t = (binary_tree*)malloc(sizeof(binary_tree));
	t->data = num;
	t->left = NULL;
	t->right = NULL;
	return t;
}
binary_tree_stern_brocot* tree_stern_brocot_node(int num, int den) 
{
	binary_tree_stern_brocot* t = (binary_tree_stern_brocot*)malloc(sizeof(binary_tree_stern_brocot));
	t->p = num;
	t->q = den; 
	t->left = NULL;
	t->right = NULL;
	return t;
}

void tree_show(const binary_tree* tree) 
{
	if (tree->left != NULL) 
	{
		tree_show(tree->left);
	}
	printf("%d ", tree->data);
	if (tree->right != NULL) 
	{
		tree_show(tree->right);
	}
}

void tree_stern_brocot_show(const binary_tree_stern_brocot* tree) 
{
	if (tree->left != NULL)
	{
		tree_stern_brocot_show(tree->left);
	}
	printf("%d/%d ", tree->p, tree->q);
	if (tree->right != NULL)
	{
		tree_stern_brocot_show(tree->right);
	}
}

void tree_free(binary_tree* tree) 
{
	if (tree == NULL)
	{
		return;
	}
	if (tree->left != NULL) 
	{
		tree_free(tree->left);
	}
	if (tree->right != NULL) 
	{
		tree_free(tree->right);
	}
	free(tree);
}

void tree_stern_brocot_free(binary_tree_stern_brocot* tree)
{
	if (tree == NULL) 
	{
		return;
	}
	if (tree->left)
	{
		tree_stern_brocot_free(tree->left);
	}
	if (tree->right)
	{
		tree_stern_brocot_free(tree->right);
	}
	free(tree);
}

char* get_stern_brocot_approximation(const binary_tree_stern_brocot *tree, double value) 
{
	binary_tree_stern_brocot* current_leaf = tree;
	char* path = NULL;
	unsigned int depth = 0;
	double fraction = 0;
	while (current_leaf)
	{
		path = (unsigned char*)realloc(path, sizeof(char) * (depth + 1));
		fraction = (double)(current_leaf->p) / (double)current_leaf->q;
		if (value < fraction)
		{
			path[depth++] = 'L';
			current_leaf = current_leaf->left;
		}
		else if (value > fraction)
		{
			path[depth++] = 'R';
			current_leaf = current_leaf->right;
		}
		else current_leaf = NULL;
		path[depth] = '\0';
	}
	return path;
}