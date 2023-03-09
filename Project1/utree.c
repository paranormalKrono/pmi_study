#include "BinaryTrees.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

void utree_add_subtree(ubinary_tree* tree, void* num);
ubinary_tree* utree_create(void** nums, int count)
{
	binary_tree* tree = utree_get_node(nums[0]);

	for (int i = 1; i < count; ++i)
		utree_add_subtree(tree, nums[i]);
	return tree;
}

void utree_add_subtree(ubinary_tree* tree, void* num)
{
	if (num < tree->data)
	{
		if (tree->left == NULL)
			tree->left = utree_get_node(num);
		else
			utree_add_subtree(tree->left, num);
	}
	else
	{
		if (tree->right == NULL)
			tree->right = utree_get_node(num);
		else
			utree_add_subtree(tree->right, num);
	}
}

ubinary_tree* utree_get_node(void* num)
{
	ubinary_tree* t = (ubinary_tree*)malloc(sizeof(ubinary_tree));
	t->data = num;
	t->left = NULL;
	t->right = NULL;
	return t;
}

// ќчищает не только дерево, но и данные в нЄм
void utree_free(ubinary_tree* tree)
{
	if (tree == NULL)
	{
		return;
	}
	if (tree->left != NULL)
	{
		utree_free(tree->left);
	}
	if (tree->right != NULL)
	{
		utree_free(tree->right);
	}
	free(tree->data);
	free(tree);
}