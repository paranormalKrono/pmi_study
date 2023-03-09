#include <stdio.h>
#include "Graph_std.h"

int graph_m_write_adj_lists(const graph_m* g, char* path);
graph_m* graph_m_read_adj_lists(char* path);

graph_m* graph_m_read(graph_std_type t, char* path)
{
	switch (t)
	{
	case Adjective_lists:
		return graph_m_read_adj_lists(path);
	/*case Adjective_matrix:
		return graph_m_read_adj_matrix(path);*/
	}
}

graph_m* graph_m_write(graph_std_type t, const graph_m* g, char* path)
{
	switch (t)
	{
	case Adjective_lists:
		return graph_m_write_adj_lists(g, path);
	/*case Adjective_matrix:
		return graph_m_write_adj_matrix(g, path);*/
	}
}

graph_m* graph_m_read_adj_lists(char* path)
{
	FILE* f;
	fopen_s(&f, path, "r");
	if (f)
	{
		int vertex_count;
		fscanf_s(f, "%d", &vertex_count);
		graph_m* g = graph_m_init(vertex_count);

		int vertex_arc = 0;
		float weight = 0;
		char cur_char = ' ';
		fgetc(f);
		for (int i = 0; i < vertex_count; ++i)
		{
			fscanf_s(f, "%d [%f] -", &vertex_arc, &weight);
			g->adj_list[i].weight = weight;
			cur_char = fgetc(f);
			while (cur_char != '\n' && cur_char != '\0' && cur_char != -1)
			{
				fscanf_s(f, "%d [%f]", &vertex_arc, &weight);
				add_m_arc(g, i, vertex_arc, weight);
				cur_char = fgetc(f);
			}
		}
		fclose(f);
		return g;
	}
	else
	{
		perror("Error");
		return NULL;
	}
}

int graph_m_write_adj_lists(const graph_m* g, char* path)
{
	FILE* f;
	fopen_s(&f, path, "w");
	if (f)
	{
		fprintf_s(f, "%d\n", g->count);
		node_m* cur_node;
		for (int i = 0; i < g->count; ++i)
		{
			fprintf(f, "%d [%.3f] -", i, g->adj_list[i].weight);
			cur_node = g->adj_list[i].head;
			while (cur_node)
			{
				fprintf_s(f, " %d [%.3f]", cur_node->vertex, cur_node->weight);
				cur_node = cur_node->next;
			}
			fprintf(f, "\n");
		}
		fclose(f);
		return 1;
	}
	else
	{
		perror("Error opening file");
		return 0;
	}
}