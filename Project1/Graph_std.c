#include <stdio.h>
#include "Graph_std.h"

graph* graph_read(char* path) 
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

int graph_write(graph* g, char* path) 
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
				fprintf_s(f, " %d", cur_node->value);
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