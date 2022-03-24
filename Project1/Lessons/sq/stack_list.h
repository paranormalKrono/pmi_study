struct stack;
typedef struct stack stack;

typedef void* element_t;

stack * stack_alloc ();
stack * stack_clone (const stack *s);
void    stack_free  (stack *s);

/* (Логические функции) */
int stack_is_empty (const stack *s);
int stack_is_full  (const stack *s);

/* 0 - успех, не 0 - неудача */
int stack_push (stack *s, element_t  e);
/* если e = NULL, игнорируется */
int stack_pop  (stack *s, element_t *e);

/* Возвращает NULL, если стек пуст */
const element_t * stack_get_front (const stack *s);
