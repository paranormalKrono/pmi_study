#include <stdio.h>
#include "array.h"

void test_int();
void test_dint();
void test_array_int();

/* Печать элементов массива (универсально) - нужен индекс для выделения последнего элемента */
void array_print (const array *a, array_icallback_f_const f)
{
    printf ("Array: size = %lu, capacity = %lu (", array_size(a), array_capacity(a));
        array_iforeach_const(a, f, NULL);
    printf (")\n");
}

/* Среднее арифметическое элементов массива (универсально) - индекс не нужен */
double array_mean (const array *a, array_callback_f_const f)
{
    double r = 0;
    array_foreach_const (a, f, &r);
    return r / array_size(a);
}


int main(void)
{
    printf ("array of ints:\n");
    test_int();

    printf ("array of dynamic ints:\n");
    test_dint();

    printf ("array of array of ints:\n");
    test_array_int();

    return 0;
}
