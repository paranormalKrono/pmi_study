#include <limits.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h> 
#include "Dbg_graphs.h"
#include "Graphs_measured.h"
#include "Fibonacci_tree.h"


void print_floats(float* results, int count) 
{
	printf_s("[");
	for (int i = 0; i < count; ++i)
	{
		printf_s("%.3f ", results[i]);
	}
	printf_s("]");
}

void initialise_weights(const graph_m* g, float edge_weight) 
{
	node_m* cur_node;
	float cur_vertex_weight;
	for (int i = 0; i < g->count; ++i)
	{
		cur_vertex_weight = 0;
		cur_node = g->adj_list[i].head;
		while (cur_node)
		{
			cur_node->weight = edge_weight;
			cur_vertex_weight += edge_weight;
			cur_node = cur_node->next;
		}
		g->adj_list[i].weight = cur_vertex_weight;
	}
}

float get_weights_sum(node_m* cur_node) 
{
	float sum = 0;
	while (cur_node)
	{
		sum += cur_node->weight; // Прибавляем веса исходящих рёбер
		cur_node = cur_node->next;
	}
	return sum;
}

void calculate_in_out_sums(graph_m* const  gm, graph_m* const gm_t, float* in_sums, float* out_sums)
{
	node_m* cur_node;
	for (int i = 0; i < gm->count; ++i)
	{
		in_sums[i] = 0;
		out_sums[i] = 0;
		
		cur_node = gm->adj_list[i].head;
		out_sums[i] = get_weights_sum(cur_node);

		cur_node = gm_t->adj_list[i].head;
		in_sums[i] = get_weights_sum(cur_node);
	}
}

node_f** fill_fibonacci_heap(fibonacci_heap* fh,  float* const in_sums,  float* const out_sums, int count)
{
	node_f** nodes = (node_f**) malloc(sizeof(node_f*) * count);
	int* datas = (int*) malloc(sizeof(float) * count);
	if (!nodes) return;
	for (int i = 0; i < count; ++i) 
	{
		datas[i] = i;
		nodes[i] = node_f_init(fabsf(in_sums[i] - out_sums[i]), &(datas[i]));
		fibonacci_heap_insert_node(fh, nodes[i]);
	}

	return nodes;
}

void print_fibonacci_heap_nodes(node_f** nodes, int count)
{
	for (int i = 0; i < count; ++i) 
	{
		print_fibonacci_heap_node(nodes[i]);
		printf_s("\n");
	}
}

float calculate_Kullback_Leibler(graph_m* const gm, graph_m* const gm_s)
{
	float res = 0;
	node_m* cur_node;
	node_m* cur_node_s;
	for (int i = 0; i < gm->count; ++i)
	{
		for (int j = 0; j < gm->count; ++j)
		{
			// Ищем путь из i в j
			cur_node = gm->adj_list[i].head;
			cur_node_s = gm_s->adj_list[i].head;
			while (cur_node && cur_node->vertex != j)
			{
				cur_node = cur_node->next;
				cur_node_s = cur_node_s->next;
			}
			if (cur_node)
				res += cur_node_s->weight * log(cur_node_s->weight / cur_node->weight);
		}
	}
	return res;
}
void check_pPp(graph_m* const gm, graph_m* const gm_s)
{
	printf_s("Прверка pP = p\n");
	float sum;
	node_m* cur_node;
	for (int j = 0; j < gm->count; ++j)
	{
		sum = 0;
		for (int i = 0; i < gm->count; ++i)
		{
			// Ищем путь из i в j
			cur_node = gm->adj_list[i].head;
			while (cur_node && cur_node->vertex != j)
				cur_node = cur_node->next;
			if (cur_node)
				sum += gm->adj_list[i].weight * cur_node->weight;
		}
		printf_s("%.3f -> %.3f\n", gm->adj_list[j].weight, sum);
	}
}

void print_Kullback_Leibler(graph_m* const gm, graph_m* const gm_start)
{
	float p = calculate_Kullback_Leibler(gm, gm_start);
	printf_s("\nКульбак-Лейблер: %.10f\n", p);
	//check_pPp(gm, gm_start);
}

void print_results(graph_m* const gm, graph_m* const gm_start, node_m** const nodes, int count, fibonacci_heap* const fh)
{
	printf_s("_-_-_RESULTS_-_-_\n");
	graph_m_print(gm);
	printf_s("_-_-_\n");
	print_Kullback_Leibler(gm, gm_start);
	printf_s("_-_-_\n");
	print_fibonacci_heap(fh);
	printf_s("_-_-_\n");
	print_fibonacci_heap_nodes(nodes, count);
	printf_s("_-_-_RESULTS_END_-_-_\n");
}

void graph_normalize(graph_m* const gm) 
{
	// Считаем сумму весов всех вершин
	float vertex_m_sum = 0;
	node_m* cur_node;
	for (int i = 0; i < gm->count; ++i) 
		vertex_m_sum += gm->adj_list[i].weight;

	// Делим веса вершин и путей на эту сумму
	for (int i = 0; i < gm->count; ++i)
	{
		gm->adj_list[i].weight /= vertex_m_sum;

		cur_node = gm->adj_list[i].head;
		while (cur_node)
		{
			cur_node->weight /= vertex_m_sum;
			cur_node = cur_node->next;
		}
	}
}

void measure_calculating()
{
	graph_m* gm = get_graph_m();
	int nodes_count = gm->count;
	if (nodes_count < 1) 
	{
		graph_m_free(gm);
		return;
	}

	initialise_weights(gm, 1);
	graph_normalize(gm);

	graph_m* gm_start = graph_m_copy(gm);
	graph_m* gm_t = graph_m_transposition(gm);

	float* in_sums = (float*)malloc(sizeof(float) * nodes_count);
	float* out_sums = (float*)malloc(sizeof(float) * nodes_count);
	if (!in_sums || !out_sums) return NULL;
	calculate_in_out_sums(gm, gm_t, in_sums, out_sums);

	graph_m_print(gm);
	printf_s("\n");
	graph_m_print(gm_t);

	fibonacci_heap* fh = fibonacci_heap_init();
	node_f** nodes = fill_fibonacci_heap(fh, in_sums, out_sums, nodes_count);

	node_m* cur_node;
	node_m* tr_node;
	int out_vertex;
	float eps = 0.0000001f;
	float gamma;

	node_f* max = fibonacci_heap_extract_max(fh);
	if (!max) 
	{
		write_graph_m(gm);
		return;
	}
	int* vi = (int*)max->data;
	if (!vi) return;
	int vertex = *vi;
	float cur_disbalance = nodes[vertex]->key;

	while (cur_disbalance > eps) 
	{
		gamma = sqrtf(out_sums[vertex] / in_sums[vertex]);
		//printf_s("vertex - %d\ndisbalance - %.2f\ngamma - %.2f\n", vertex, cur_disbalance, gamma);

		// Исходящие пути в графе
		cur_node = gm->adj_list[vertex].head;
		while (cur_node)
		{
			// Ищем соответствующее ребро в транспонированном графе
			out_vertex = cur_node->vertex;
			tr_node = gm_t->adj_list[out_vertex].head;
			while (tr_node->vertex != vertex) tr_node = tr_node->next;

			cur_node->weight /= gamma;
			tr_node->weight = cur_node->weight;

			// Пересчёт суммы входящих путей для вершины в конце пути
			in_sums[out_vertex] = get_weights_sum(gm_t->adj_list[out_vertex].head);

			// Удаляем выходящую вершину из списка, меняем её дисбаланс и вставляем обратно
			fibonacci_heap_delete_key(fh, nodes[out_vertex]);
			nodes[out_vertex]->key = fabsf(out_sums[out_vertex] - in_sums[out_vertex]);
			fibonacci_heap_insert_node(fh, nodes[out_vertex]);

			cur_node = cur_node->next;
		}

		// Входящие пути в транспонированном графе
		cur_node = gm_t->adj_list[vertex].head;
		while (cur_node)
		{
			// Ищем соответствующее ребро в обычном графе
			out_vertex = cur_node->vertex;
			tr_node = gm->adj_list[out_vertex].head;
			while (tr_node->vertex != vertex) tr_node = tr_node->next;

			cur_node->weight *= gamma;
			tr_node->weight = cur_node->weight;

			out_sums[out_vertex] = get_weights_sum(gm->adj_list[out_vertex].head);

			fibonacci_heap_delete_key(fh, nodes[out_vertex]);
			nodes[out_vertex]->key = fabsf(out_sums[out_vertex] - in_sums[out_vertex]);
			fibonacci_heap_insert_node(fh, nodes[out_vertex]);

			cur_node = cur_node->next;
		}

		out_sums[vertex] /= gamma; //  Меняем сумму выходящих рёбер
		in_sums[vertex] *= gamma; // Меняем сумму входящих рёбер

		nodes[vertex]->key = fabsf(out_sums[vertex] - in_sums[vertex]);
		fibonacci_heap_insert_node(fh, nodes[vertex]);

		max = fibonacci_heap_extract_max(fh);
		if (!max)
		{
			write_graph_m(gm);
			return;
		}
		vi = (int*)max->data;
		vertex = *vi;
		cur_disbalance = nodes[vertex]->key;
	}

	for (int i = 0; i < nodes_count; ++i) 
	{
		gm->adj_list[i].weight = out_sums[i];
		gm_t->adj_list[i].weight = in_sums[i];
	}


	graph_normalize(gm);
	print_results(gm, gm_start, nodes, nodes_count, fh);

	write_graph_m(gm);
	graph_m_free(gm_start);
	graph_m_free(gm_t);
	graph_m_free(gm);
}
