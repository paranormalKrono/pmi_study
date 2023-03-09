#include "Complex_tree.h"
#include "Small_exercises.h"
#include <limits.h>
#include <stdio.h>
#include "BinaryTrees.h"
#include "Complex_numbers.h"

void show_complex() 
{
	complex_number* cn = complex_create(85, 60); // ����������� �����
	int depth = 4; // ������� ������

	ubinary_tree* complex_tree = get_complex_tree(cn, depth);
	show_complex_tree(complex_tree);

	utree_free(complex_tree);
}