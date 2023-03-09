#include <malloc.h>
#include <memory.h>
#include "Fibonacci_tree.h"
#include <float.h>
#include <stdio.h>
#include <math.h>

void print_fibonacci_heap_node(node_f* cur_node)
{
	if (!cur_node)
	{
		printf_s("NULL\n");
		return;
	}
	if (cur_node->parent)
		printf_s("[%d] ", *((int*)cur_node->parent->data));
	else
		printf_s("[N] ");
	
	if (cur_node->mark) printf_s("+ ");
	else printf_s("- ");

	printf_s("[%d - %.10f - %d] ",
		*((int*)cur_node->data), cur_node->key, cur_node->degree);

	if (cur_node->left)
		printf_s("[%d] ", *((int*)cur_node->left->data));
	else
		printf_s("[N] ");

	if (cur_node->right)
		printf_s("[%d] ", *((int*)cur_node->right->data));
	else
			printf_s("[N] ");

	if (cur_node->child)
		printf_s("[%d]", *((int*)cur_node->child->data));
	else
		printf_s("[N]");

}

int print_fibonacci_heap_root(int count, node_f* const root, node_f* const parent, int tabs_count)
{
	node_f* cur_node = root;
	node_f* prev_node;
	bool is_correct = true;
	int cur_count = 0;
	int childs_count;
	if (!cur_node) 
	{
		printf_s("NULL\n");
		return 0;
	}
	else
		prev_node = cur_node->left;
	do
	{
		for (int i = 0; i < tabs_count*2; ++i)
			printf_s("\t");

		if (cur_node->parent != parent)
		{
			printf_s("[Ошибка] неверный родитель ");
			is_correct = false;
		}

		if (cur_node->right == NULL || prev_node != cur_node->left || prev_node->right != cur_node)
		{
			printf_s("[Критическая ошибка] неверный указатель в двусвязном списке\n");
			printf_s("left\n\tleft->right\n\tcur->left\ncur\n");
			print_fibonacci_heap_node(prev_node);
			printf_s("\n\t");
			print_fibonacci_heap_node(prev_node->right);
			printf_s("\n\t");
			print_fibonacci_heap_node(cur_node->left);
			printf_s("\n");
			print_fibonacci_heap_node(cur_node);
			printf_s("\n");
			return -1;
		}
		else 
		{
			print_fibonacci_heap_node(cur_node);
			printf_s("\n");
		}

		if (cur_node->child != NULL) 
		{
			childs_count = print_fibonacci_heap_root(count, cur_node->child, cur_node, tabs_count + 1);
			if (childs_count == -1)
				return -1;
			else
				cur_count += childs_count;
		}

		cur_count++;
		prev_node = prev_node->right;
		cur_node = cur_node->right;
	} while (cur_count < count + 1 && cur_node != root);

	if (cur_count == count + 1)
	{
		printf_s("[Критическая ошибка] разорван цикл в двусвязном списке\n");
		return -1;
	}
	return cur_count;
}

int fibonacci_heap_check_max(node_f* const root, node_f* const max)
{
	node_f* cur_node = root;
	node_f* cur_max = max;
	bool is_max = false; // Есть ли этот max в дереве
	if (!cur_max)
	{
		printf_s("[Ошибка] нет максимального ключа\n");
		return false;
	}
	do
	{
		if (cur_node == max) is_max = true;
		if (cur_node->key > max->key)
			cur_max = cur_node;
		cur_node = cur_node->right;
	} while (cur_node != cur_node);

	if (!is_max) 
	{
		printf_s("[Критическая ошибка] нет максимума в списке\n");
		return -1;
	}
	if (cur_max->key > max->key)
	{
		printf_s("[Ошибка] неверный максимальный ключ. Максимален: ");
		print_fibonacci_heap_node(cur_max);
		printf_s("\n");
		return false;
	}
	return true;
}

void print_fibonacci_heap(fibonacci_heap* fh)
{
	printf_s("fibonacci_heap: [parent] is_marked [data - key - degree] [left] [right] [child]\n");
	printf_s("[nodes_count] %d\n", fh->n_count);
	printf_s("[max] ");
	print_fibonacci_heap_node(fh->max);
	printf_s("\n[root]\n");
	bool is_success = true;
	int is_max;
	int count = print_fibonacci_heap_root(fh->n_count, fh->root, NULL, 0);
	if (count == -1) 
	{
		printf_s("FAILURE\n");
		is_success = false;
		print_fibonacci_heap(fh);
	}
	else
	{
		is_max = fibonacci_heap_check_max(fh->root, fh->max);
		if (is_max == -1)
			printf_s("[FAILURE]\n");
		else if (count != fh->n_count)
		{
			printf_s("[Ошибка] несовпадение количества элементов. Количество: %d\n", count);
			printf_s("[MAYBE_SUCCESS]\n");
		}
		else if (is_max == 0)
			printf_s("[MAYBE_SUCCESS]\n");
		else printf_s("[SUCCESS]\n");
	}
	printf_s("\n");
}

node_f* node_f_init(float key, void* data)
{
	node_f* n = (node_f*)malloc(sizeof(node_f));
	if (n)
	{
		n->data = data;

		n->key = key;
		n->degree = 0;
		n->mark = false;
		n->parent = NULL;
		n->child = NULL;
		n->left = NULL;
		n->right = NULL;
	}
	return n;
}

fibonacci_heap* fibonacci_heap_init()
{
	fibonacci_heap* fh = (fibonacci_heap*)malloc(sizeof(fibonacci_heap));
	if (fh)
	{
		fh->n_count = 0;
		fh->max = NULL;
		fh->root = NULL;
	}
	return fh;
}

void merge(fibonacci_heap* fh, node_f* n)
{
	if (!fh->root)
	{
		fh->root = n;
	}
	else
	{
		n->right = fh->root;
		n->left = fh->root->left;
		fh->root->left->right = n;
		fh->root->left = n;
	}
}

void merge_list(fibonacci_heap* fh, node_f* n)
{
	if (!fh->root)
	{
		fh->root = n;
	}
	else
	{
		fh->root->left->right = n;
		n->left->right = fh->root;

		node_f* tmp = n->left;
		tmp = fh->root->left;
		fh->root->left = n->left;
		n->left = tmp;
	}
}

void fibonacci_heap_insert_node(fibonacci_heap* fh, node_f* node)
{
	//printf_s("\n--{INSERT_NODE}-- node - ");
	//print_fibonacci_heap_node(node);
	//printf_s("\n");
	node->left = node;
	node->right = node;
	node->parent = NULL;

	merge(fh, node);

	if (!fh->max || node->key > fh->max->key)
	{
		fh->max = node;
	}

	fh->n_count++;

	//printf_s("--{INSERT_NODE_END}--\n");
	//print_fibonacci_heap(fh, false);
}

node_f* fibonacci_heap_insert(fibonacci_heap* fh, float key, void* data)
{
	node_f* node = node_f_init(key, data);
	fibonacci_heap_insert_node(fh, node);
	return node;
}

fibonacci_heap* fibonacci_heap_union(fibonacci_heap* fh1, fibonacci_heap* fh2)
{
	fibonacci_heap* fh = fibonacci_heap_init();

	fh->root = fh1->root;

	// Устанавливаем максимум
	if (fh1->max > fh2->max)
		fh->max = fh1->max;
	else fh->max = fh2->max;

	merge_list(fh, fh2->root);

	fh->n_count = fh1->n_count + fh2->n_count;

	return fh;
}


node_f* fibonacci_heap_link(node_f* nf1, node_f* nf2) // nf1.degree = nf2.degree
{
	if (nf1->key < nf2->key)
	{
		return fibonacci_heap_link(nf2, nf1);
	}

	// Убрали nf2 из дерева
	nf2->left->right = nf2->right;
	nf2->right->left = nf2->left;

	// Вставляем nf2 как ребёнка с БОльшим ключом
	if (!nf1->child)
	{
		nf2->left = nf2;
		nf2->right = nf2;

		nf1->child = nf2;
	}
	else
	{
		nf2->left = nf1->child->left;
		nf2->right = nf1->child;

		nf1->child->left->right = nf2;
		nf1->child->left = nf2;
	}

	nf2->parent = nf1;
	nf2->mark = false;
	nf1->degree++;
	
	return nf1;
}

int get_max_degree(fibonacci_heap* fh) 
{
	return (int)(log((float)fh->n_count)/log((1+sqrt(5))/2)); // loga(b) = log(b)/log(a), золотое сечение
}

void fibonacci_heap_consolidate(fibonacci_heap* fh)
{
	//printf_s("--{CONSOLIDATING}--\n");
	//print_fibonacci_heap(fh, true);

	int max_degree = get_max_degree(fh);
	node_f** trees_by_degrees = (node_f**)malloc(sizeof(node_f*) * max_degree);
	if (!trees_by_degrees) return;
	for (int i = 0; i < max_degree; ++i) 
	{
		trees_by_degrees[i] = NULL;
	}


	node_f* merge_node;
	node_f* end_node = fh->root->left;
	node_f* cur_n = fh->root, *new_n;
	bool is_stop = false;
	while (!is_stop)
	{
		if (cur_n == end_node) is_stop = true;
		new_n = cur_n;
		cur_n = cur_n->right;
		while (trees_by_degrees[new_n->degree])
		{
			merge_node = trees_by_degrees[new_n->degree];
			trees_by_degrees[new_n->degree] = NULL; // убрали merge дерево из списка

			new_n = fibonacci_heap_link(new_n, merge_node); // mergим деревья с одинаковым degree
		}
		// Все ноды до cur_n изменились, но конец списка остался тот же

		if (fh->max == NULL || fh->max->key <= new_n->key)
		{
			fh->root = fh->max = new_n;
		}
		trees_by_degrees[new_n->degree] = new_n;

	}
	//printf_s("--{CONSOLIDATING_END}--\n");
}

node_f* fibonacci_heap_extract_max(fibonacci_heap* fh)
{
	//printf_s("\n--{EXTRACT_MAX}--\n");

	node_f* max = fh->max;
	if (!max) return NULL;

	if (fh->root == max)
	{
		//Меняем root
		if (max == max->right) fh->root = NULL;
		else fh->root = max->left;
	}

	// убираем max
	max->left->right = max->right;
	max->right->left = max->left;
	fh->max = NULL;

	node_f* root = fh->root;

	if (max->child)
	{
		// Вставляем детей в root список
		merge_list(fh, max->child);

		node_f* cur_node = max->child;
		do // Убираем родителей у всех детей
		{
			cur_node->parent = NULL;
			cur_node = cur_node->right;
		} while (cur_node != max->child);
	}
	

	if (max != max->right) // Если в root списке был не только max
		fibonacci_heap_consolidate(fh);

	max->degree = 0;
	max->mark = false;
	max->left = NULL;
	max->right = NULL;
	max->child = NULL;
	max->parent = NULL;

	fh->n_count -= 1;

	//printf_s("--{EXTRACT_END}-- result - ");
	//print_fibonacci_heap_node(max);
	//printf_s("\n");
	//print_fibonacci_heap(fh, false);
	return max;
}

void cut(fibonacci_heap* fh, node_f* x, node_f* y) 
{
	if (x->left == x) y->child = NULL;
	else 
	{
		if (y->child == x) y->child = x->left;
		x->left->right = x->right;
		x->right->left = x->left;
	}
	y->degree--;

	x->left = fh->root->left;
	x->right = fh->root;

	fh->root->left->right = x;
	fh->root->left = x;
	x->parent = NULL;
	x->mark = false;
}

void cascading_cut(fibonacci_heap* fh, node_f* y) 
{
	node_f* parent = y->parent;
	if (parent) 
	{
		if (!y->mark) y->mark = true;
		else 
		{
			cut(fh, y, parent);
			cascading_cut(fh, parent);
		}
	}
}

void fibonacci_heap_increase_key(fibonacci_heap *fh, node_f* x, float new_value) 
{
	//printf_s("--{INCREASE_KEY}--\n");
	//print_fibonacci_heap_node(x);
	//printf_s("\nnew_value - %.2f\n", new_value);
	if (!fh->max)
	{
		printf_s("[Критическая ошибка] нет максимального ключа\n");
		return;
	}

	if (new_value < x->key) return;
	x->key = new_value;
	node_f* parent = x->parent;
	if (parent && x->key > parent->key) 
	{
		cut(fh, x, parent);
		cascading_cut(fh, parent);
	}
	if (x->key > fh->max->key) fh->max = x;

	//printf_s("--{INCREASE_KEY_END}--\n");
}

node_f* fibonacci_heap_delete_key(fibonacci_heap* fh, node_f* x) 
{
	//printf_s("\n--{DELETE_KEY}-- node - ");
	//print_fibonacci_heap_node(x);
	//printf_s("\n");

	fibonacci_heap_increase_key(fh, x, FLT_MAX);
	node_f* nf = fibonacci_heap_extract_max(fh);

	//printf_s("--{DELETE_KEY_END}--\n");
	//print_fibonacci_heap(fh, false);
	return nf;
}