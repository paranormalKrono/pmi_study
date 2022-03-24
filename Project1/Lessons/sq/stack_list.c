#include <stdlib.h>
#include "stack_list.h"

struct node
{
    element_t data;
    struct node *next;
};
typedef struct node node;

struct stack
{
    node *top;
};

stack * stack_alloc ()
{
    stack *s = malloc(sizeof (*s));
    if (!s) return NULL;

    s->top = NULL;

    return s;
}
stack * stack_clone (const stack *s)
{
    stack *r = malloc(sizeof(*r));

    node *t = s->top;
    node **n = &(r->top);

    if (!r) return NULL;

    while (t != NULL)
    {
        *n = malloc(sizeof(node));
        if (!*n)
        {
            stack_free(r);
            return NULL;
        }

        (*n)->data = t->data;
        (*n)->next = NULL;

        n = &((*n)->next);

        t = t->next;
    }

    return r;
}

void    stack_free  (stack *s)
{
    while (stack_pop(s, NULL));
    free(s);
}

/* (Логические функции) */
int stack_is_empty (const stack *s)
{
    return s->top == NULL;
}
int stack_is_full  (const stack *s)
{
    return 0;
}

/* 0 - успех, не 0 - неудача */
int stack_push (stack *s, element_t  e)
{
    node *n = malloc (sizeof(*n));
    if (!n) return 0;

    n->data = e;
    n->next = s->top;
    s->top = n;

    return 1;
}

/* если e = NULL, игнорируется */
int stack_pop  (stack *s, element_t *e)
{
    node *t = s->top;
    if (s->top == NULL) return 0;

    s->top = t->next;
    if (e) *e = t->data;
    free(t);

    return 1;
}

/* Возвращает NULL, если стек пуст */
const element_t * stack_get_front (const stack *s)
{
    return s->top == NULL ? NULL : &(s->top->data);
}
