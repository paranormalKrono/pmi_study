#include "Complex_tree.h"
#include <limits.h>
#include <stdio.h>
#include <malloc.h>
#include "Lessons/sq/stack_list.h"


ubinary_tree* get_complex_tree(complex_number* cn, int depth)
{
	ubinary_tree* utree = utree_get_node(cn);

	depth--;
	if (depth > 0)
	{
		complex_number** cprotos = get_complex_protos(cn, 2);
		utree->left = get_complex_tree(cprotos[0], depth);
		utree->right = get_complex_tree(cprotos[1], depth);
	}

	return utree;
}

void show_complex_sub(const ubinary_tree* tree, int sublevel)
{
	if (tree->left != NULL)
	{
		show_complex_sub(tree->left, sublevel + 1);
	}
	complex_number* cn = (complex_number*)(tree->data);
	for (int i = 0; i < sublevel; ++i) printf_s("\t");
	printf_s("[");
	if (cn->real != 0) printf_s("%.3f", cn->real);
	if (cn->imaginary != 0)
	{
		printf_s(" ");
		if (cn->imaginary == 1) printf_s("+ i");
		else if (cn->imaginary == -1)  printf_s("- i");
		else if (cn->imaginary > 0)
			printf_s("+ %.3fi", cn->imaginary);
		else
			printf_s("- %.3fi", -cn->imaginary);
	}
	printf_s("]\n");

	if (tree->right != NULL)
	{
		show_complex_sub(tree->right, sublevel + 1);
	}
}



void show_complex_tree(const ubinary_tree* complex_tree)
{
	show_complex_sub(complex_tree, 0);
	printf_s("\n");
}