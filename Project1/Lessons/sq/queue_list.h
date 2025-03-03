#pragma once

struct queue;
typedef struct queue queue;

typedef void* element_t;

queue * queue_alloc ();
queue * queue_clone (const queue *q);
void    queue_free  (queue *q);

int queue_is_empty (const queue *q);
int queue_is_full  (const queue *q);

int queue_push (queue *s, element_t  q);
int queue_pop  (queue *s, element_t *q);

const element_t * queue_get_front (const queue *q);

// generic stack c size; void*
