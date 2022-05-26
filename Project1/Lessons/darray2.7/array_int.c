#include "array.h"
#include <stdio.h>
#include <string.h>

void array_print (const array *a, array_icallback_f_const f);
double array_mean (const array *a, array_callback_f_const f);

/* Печать элемента int */
void print_int (const void *e, size_t i, size_t n, void *data)
{
    (void) data;
    printf ("%i%s", *(int*) e, i == n - 1 ? "" : ", ");
}

/* Добавка элемента int */
void sum_int (const void *e, void *data)
{
    *(double *)data += (double) *(int *)e;
}

void test_int()
{
    array_pars pars;
    array *b, *a;
    int n;

    memset(&pars, 0, sizeof(pars)); /* установим все в 0/NULL, затем зададим лишь нужные параметры */
    pars.min = 3;

    a = array_alloc (sizeof(int), &pars);


    n = 1; array_insert (a, &n);  array_print (a, print_int);
    n = 2; array_insert (a, &n);  array_print (a, print_int);
    n = 3; array_insert (a, &n);  array_print (a, print_int);
    n = 4; array_insert (a, &n);  array_print (a, print_int);
    n = 5; array_insert (a, &n);  array_print (a, print_int);
    n = 6; array_insert (a, &n);  array_print (a, print_int);

    b = array_clone(a);

    n = 7; array_insert (a, &n);  array_print (a, print_int);
    n = 8; array_insert (a, &n);  array_print (a, print_int);
    n = 9; array_insert (a, &n);  array_print (a, print_int);

    printf ("Mean value = %f\n", array_mean(a, sum_int));

    array_delete (a, 2);  array_print (a, print_int);
    array_delete (a, 1);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);
    array_delete (a, 0);  array_print (a, print_int);

    array_free(a);

    array_print(b, print_int);
    printf ("Mean value = %f\n", array_mean(b, sum_int));

    array_free(b);

}
