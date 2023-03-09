#include <stdio.h>
#include "Graph_std.h"

graph* graph_read_adj_lists(char* path);
int graph_write_adj_lists(const graph* g, char* path);
graph* graph_read_adj_matrix(char* path);
int graph_write_adj_matrix(const graph* g, char* path);

graph* graph_read(graph_std_type t, char* path) 
{
	switch (t)
	{
	case Adjective_lists:
		return graph_read_adj_lists(path);
	case Adjective_matrix:
		return graph_read_adj_matrix(path);
	}
}

graph* graph_write(graph_std_type t, const graph* g, char* path)
{
	switch (t)
	{
	case Adjective_lists:
		return graph_write_adj_lists(g, path);
	case Adjective_matrix:
		return graph_write_adj_matrix(g, path);
	}
}

graph* graph_read_adj_lists(char* path) 
{
	FILE* f;
	fopen_s(&f, path, "r");
	if (f)
	{
		int vertex_count;
		fscanf_s(f, "%d", &vertex_count);
		graph* g = graph_init(vertex_count);

		int vertex_arc = 0;
		char cur_char = ' ';
		fgetc(f);
		for (int i = 0; i < vertex_count; ++i)
		{
			fscanf_s(f, "%d -", &vertex_arc);
			cur_char = fgetc(f);
			while (cur_char != '\n' && cur_char != '\0' && cur_char != -1)
			{
				fscanf_s(f, "%d", &vertex_arc);
				add_arc(g, i, vertex_arc);
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

int graph_write_adj_lists(const graph* g, char* path)
{
	FILE* f;
	fopen_s(&f, path, "w");
	if (f)
	{
		fprintf_s(f, "%d\n", g->count);
		node* cur_node;
		for (int i = 0; i < g->count; ++i)
		{
			fprintf(f, "%d -", i);
			cur_node = g->adj_list[i].head;
			while (cur_node)
			{
				fprintf_s(f, " %d", cur_node->vertex);
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

graph* graph_read_adj_matrix(char* path)
{
	FILE* f;
	fopen_s(&f, path, "r");
	if (f)
	{
		int vertex_count;
		fscanf_s(f, "%d", &vertex_count);
		graph* g = graph_init(vertex_count);

		int isArc = 0;
		char cur_char = ' ';
		for (int i = 0; i < vertex_count; ++i)
		{
			for (int j = 0; j < vertex_count; ++j) 
			{
				fscanf_s(f, "%d", &isArc);
				if (isArc)
					add_arc(g, i, j);

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

int graph_write_adj_matrix(const graph* g, char* path) 
{
	FILE* f;
	fopen_s(&f, path, "w");
	if (f)
	{
		fprintf_s(f, "%d\n", g->count);
		node* cur_node;
		for (int i = 0; i < g->count; ++i)
		{
			for (int j = 0; j < g->count; ++j)
			{
				cur_node = g->adj_list[i].head;
				while (cur_node && cur_node->vertex != j)
					cur_node = cur_node->next;
				if (cur_node && cur_node->vertex == j)
					fprintf_s(f, "%d ", 1);
				else
					fprintf_s(f, "%d ", 0);
			}
			fprintf_s(f, "\n");
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