#include <stdlib.h>
#include "stack_list.h"

typedef struct node
{
    element_t data;
    struct node *next;
} node;

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

stack * stack_clone (const stack *before)
{
    if (!before) return NULL;

    stack* clone = stack_alloc();
    if (!clone) return NULL;

    node* cur_node = before->top;
    if (!cur_node) return clone;

    node* cur_clone_node = clone->top = malloc(sizeof(node));
    cur_clone_node->data = cur_node->data;
    cur_clone_node->next = NULL;
    

    while (cur_node->next)
    {
        cur_node = cur_node->next;
        cur_clone_node = cur_clone_node->next = malloc(sizeof(node));
        cur_clone_node->data = cur_node->data;
        cur_clone_node->next = NULL;
    }

    return clone;
}

void stack_free (stack *s)
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
int stack_pop  (stack *st, element_t *el)
{
    node *cur_node = st->top;
    if (!cur_node) return 0;

    st->top = cur_node->next;
    if (el) *el = cur_node->data;
    free(cur_node);

    return 1;
}

/* Возвращает NULL, если стек пуст */
const element_t * stack_get_front (const stack *s)
{
    return s->top == NULL ? NULL : &(s->top->data);
}
