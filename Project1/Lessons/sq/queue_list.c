#include <stdlib.h>
#include "queue_list.h"

struct node
{
    element_t data;
    struct node *next;
};
typedef struct node node;

struct queue
{
    node *head, *tail;
};

queue * queue_alloc ()
{
    queue *q = malloc(sizeof(*q));
    if (!q) return NULL;

    q->head = q->tail = NULL;
    return q;
}
queue * queue_clone (const queue *q)
{
    queue *r = queue_alloc();
    node *t = q->head;
    if (!r) return NULL;

    while (t != NULL)
    {
        if (!queue_push(r, t->data))
        {
            queue_free(r);
            return NULL;
        }
        t = t->next;
    }

    return r;
}
void    queue_free  (queue *q)
{
    while (queue_pop(q, NULL));
    free(q);
}

int queue_is_empty (const queue *q)
{
    return q->head == NULL;
}
int queue_is_full  (const queue *q)
{
    return 0;
}

int queue_push (queue *q, element_t e)
{
    node *n = malloc(sizeof(*n));
    if (!n) return 0;

    n->data = e;
    n->next = NULL;

    if (q->head == NULL)
        q->head = n;
    else
        q->tail->next = n;

    q->tail = n;

    return 1;
}
int queue_pop  (queue *q, element_t *e)
{
    node *t = q->head;
    if (!t) return 0;

    q->head = t->next;
    if (e) *e = t->data;
    free(t);

    if (q->head == NULL) q->tail = NULL;

    return 1;
}

const element_t * queue_get_front (const queue *q)
{
    return q->head == NULL ? NULL : &(q->head->data);
}

