#include "array.h"
#include <stdio.h>
#include <string.h>

typedef struct array_int
{
    array *a;
} array_int;

void array_print (const array *a, array_icallback_f_const f);

/* Печать элементов массива (универсально) - нужен индекс для выделения последнего элемента */
void array_print_internal (const array *a, array_icallback_f_const f, size_t i, size_t n)
{
    printf ("array: size = %lu, capacity = %lu [", array_size(a), array_capacity(a));
        array_iforeach_const(a, f, NULL);
    printf ("]");
    if (i != n - 1) printf (", ");
}


/* Печать элемента int */
void print_int (const void *e, size_t i, size_t n, void *data);

/* Печать элемента array_int */
void print_array_int (const void *e, size_t i, size_t n, void *data)
{
    (void) data;
    array_print_internal(((array_int *)e)->a, print_int, i, n);
}

int array_int_fa (void *e, void *data)
{
    array_int *a = e;
    a->a = array_alloc (sizeof(int), data);
    if (!a->a) return 1;
    return 0;
}

void array_int_ff (void *e, void *data)
{
    array_int *a = e;
    array_free(a->a);
    (void) data;
}

int array_int_fc (void *dst, const void *src, void *data)
{
    array_int *d = dst;
    const array_int *s = src;
    d->a = array_clone(s->a);
    if (!d->a) return 1;
    (void) data;
    return 0;
}


void test_array_int()
{
    array_pars int_pars, array_int_pars;
    array *a, *b;
    int n;
    array_int p, *q;

    memset(&int_pars, 0, sizeof(int_pars)); /* установим все в 0/NULL, затем зададим лишь нужные параметры */
    int_pars.min = 1;

    memset(&array_int_pars, 0, sizeof(array_int_pars)); /* установим все в 0/NULL, затем зададим лишь нужные параметры */
    array_int_pars.min = 1;
    array_int_pars.fa  = array_int_fa;
    array_int_pars.da  = &int_pars;     /* создаем внутренние массивы с этими параметрами */
    array_int_pars.ff  = array_int_ff;
    array_int_pars.fc  = array_int_fc;

    p.a = array_alloc (sizeof(int), &int_pars); /* внутренний массив */

    a = array_alloc (sizeof(p), &array_int_pars);

    n = 1; array_insert (p.a, &n);
    n = 2; array_insert (p.a, &n);
    n = 3; array_insert (p.a, &n);
    array_insert (a, &p); array_print (a, print_array_int);

    n = 4; array_insert (p.a, &n);
    n = 5; array_insert (p.a, &n);
    n = 6; array_insert (p.a, &n);
    n = 7; array_insert (p.a, &n);
    array_insert (a, &p); array_print (a, print_array_int);

    b = array_clone(a);

    n = 8; array_insert (p.a, &n);
    n = 9; array_insert (p.a, &n);
    array_insert (a, &p); array_print (a, print_array_int);

    array_delete (a, 2);  array_print (a, print_array_int);

    /* получаем указатель на внутренний массив с индексом 1 */
    q = array_element(a, 1);
    /* модифицируем */
    n = 5;
    array_set (q->a, 1, &n);
    /* измененный массив массивов */
    array_print (a, print_array_int);

    array_delete (a, 0);  array_print (a, print_array_int);
    array_delete (a, 0);  array_print (a, print_array_int);

    array_free(a);

    array_print(b, print_array_int);

    array_free(b);

    array_free(p.a);

}
