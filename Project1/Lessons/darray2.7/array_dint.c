#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_print (const array *a, array_icallback_f_const f);
double array_mean (const array *a, array_callback_f_const f);


typedef struct dint
{
    int *n;
} dint;

int dint_fa (void *e, void *data)
{
    dint *i = e;
    (void) data;
    i->n = malloc(sizeof(int));
    if (!i->n) return 1;
    return 0;
}

void dint_ff (void *e, void *data)
{
    dint *i = e;
    (void) data;
    free (i->n);
}

int dint_fc (void *dst, const void *src, void *data)
{
    dint *d = dst; const dint *s = src;
    (void) data;
    d->n = malloc (sizeof(int));
    if (!d->n) return 1;
    *(d->n) = *(s->n);
    return 0;
}

/* Печать элемента int */
void print_dint (const void *e, size_t i, size_t n, void *data)
{
    (void) data;
    printf ("%i%s", *((dint*) e)->n, i == n - 1 ? "" : ", ");
}

/* Добавка элемента int */
void sum_dint (const void *e, void *data)
{
    *(double *)data += (double) *((dint *)e)->n;
}

void test_dint()
{
    array_pars pars;
    array *b, *a;
    dint i;

    memset(&pars, 0, sizeof(pars)); /* установим все в 0/NULL, затем зададим лишь нужные параметры */
    pars.min = 3;
    pars.fa  = dint_fa;
    pars.ff  = dint_ff;
    pars.fc  = dint_fc;

    a = array_alloc (sizeof(dint), &pars);
    i.n = malloc(sizeof (int));

    *i.n = 1; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 2; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 3; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 4; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 5; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 6; array_insert (a, &i);  array_print (a, print_dint);

    b = array_clone(a);

    *i.n = 7; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 8; array_insert (a, &i);  array_print (a, print_dint);
    *i.n = 9; array_insert (a, &i);  array_print (a, print_dint);

    printf ("Mean value = %f\n", array_mean(a, sum_dint));

    array_delete (a, 2);  array_print (a, print_dint);
    array_delete (a, 1);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);
    array_delete (a, 0);  array_print (a, print_dint);

    array_free(a);

    array_print(b, print_dint);
    printf ("Mean value = %f\n", array_mean(b, sum_dint));

    array_free(b);

    free(i.n);
}
