#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"

struct array
{
    void *data;          /* собственно массив данных                              */
    size_t count;        /* размер массива                                        */
    size_t size;         /* размер элемента                                       */
    size_t capacity;     /* вместимость                                           */
    size_t min;          /* минимальная вместимость                               */
    array_capacity_f fr; /* устанавливаемая вместимость по размеру массива        */
    array_capacity_f fs; /* максимально допустимая вместимость по размеру массива */
    array_alloc_f    fa; /* функция создания элемента массива                     */
    void            *da; /* данные создания элемента                              */
    array_free_f     ff; /* функция удаления элемента массива                     */
    void            *df; /* данные удаления элемента                              */
    array_clone_f    fc; /* функция клонирования элемента массива                 */
    void            *dc; /* данные клонирования элемента                          */
};

size_t array_capacity_fr (size_t count)
{
    return 2 * count;
}

size_t array_capacity_fs (size_t count)
{
    return 4 * count;
}

int array_fa (void *e, void *data)
{
    (void) e;
    (void) data;
    return 0;
}

void array_ff (void *e, void *data)
{
    (void) e;
    (void) data;
}

int array_fc (void *dst, const void *src, void *data)
{
    memcpy (dst, src, *(size_t *) data);
    return 0;
}

array  *array_alloc (size_t size, const array_pars *pars)
{
    array *a = malloc (sizeof *a);
    if (!a) return NULL;

    a->count    = 0;
    a->size     = size;
    a->data     = NULL;

    /* Присваиваем значения по умолчанию или заданные */
    a->min      = !pars || !pars->min ? ARRAY_MIN_CAPACITY : pars->min;
    a->fr       = !pars || !pars->fr  ? array_capacity_fr  : pars->fr;
    a->fs       = !pars || !pars->fs  ? array_capacity_fs  : pars->fs;
    a->fa       = !pars || !pars->fa  ? array_fa           : pars->fa;
    a->ff       = !pars || !pars->ff  ? array_ff           : pars->ff;
    a->fc       = !pars || !pars->fc  ? array_fc           : pars->fc;
    a->da       = !pars               ? NULL               : pars->da;
    a->df       = !pars               ? NULL               : pars->df;
    a->dc       = !pars || !pars->fc  ? &a->size           : pars->dc;

    array_adjust_capacity (a);
    return a;
}

array *array_clone (const array *a)
{
    const char *i, *e;
    char *j;

    array_pars pars;
    array *b;

    pars.min = a->min;
    pars.fr  = a->fr;
    pars.fs  = a->fs;
    pars.fa  = a->fa;
    pars.da  = a->da;
    pars.ff  = a->ff;
    pars.df  = a->df;
    pars.fc  = a->dc == &a->size ? NULL : a->fc; /* если используется memcpy, alloc должен выставить указатель на size копии */
    pars.dc  = a->dc == &a->size ? NULL : a->dc;

    b = array_alloc(a->size, &pars);

    if (!b) return NULL;

    if (array_set_capacity(b, a->capacity))
    {
        array_free(b);
        return NULL;
    }
    b->count = a->count;

    for (i = a->data, j = b->data, e = (char *) a->data + a->count * a->size;
         i < e;
         i += a->size, j += b->size
        ) /* проходим элементы двух массивов */
    {
        if (a->fc (j, i, b->dc)) /* копируем */
        {
            /* ошибка копирования, откат */
            b->count = (i - (char *)a->data) / a->size; /* сколько успели скопированить - free освободит по count */
            array_free(b);
            return NULL;
        }
    }
    return b;
}


void array_free (array *a)
{
    char *i, *e;
    for (i = a->data, e = (char *) a->data + a->count * a->size;
         i < e;
         i += a->size
        ) a->ff (i, a->df);
    free (a->data);
    free (a);
}

size_t array_sizeof() {return sizeof(array); }


int array_set_capacity (array *a, size_t capacity)
{
    char *i, *e;
    void *n;

    if (a->count > capacity) /* нужно удалить элементы */
    {
        for (i = (char *) a->data + a->count * a->size, e = (char *) a->data + capacity * a->size;
             i < e;
             i += a->size
            ) a->ff(i, a->df);
        n = realloc (a->data, capacity * a->size); /* уменьшение вместимости */
        if (n) a->data = n;
        /* в случае неудачи объем памяти остается старым, но capacity становится меньшим
         * к "лишним" элементам обращений не будет
         * откат невозможен
         */
         a->count = capacity;
    }
    else
    {
        n = realloc (a->data, capacity * a->size);
        if (!n) return 1; /* выделить или сократить память неудалось */
        a->data = n;
    }

    a->capacity = capacity;
    return 0;
}

int array_adjust_capacity (array *a)
{
    size_t capacity = a->fr (a->count);
    if (capacity < a->min) capacity = a->min;
    return array_set_capacity (a, capacity);
}

int array_set_min_capacity (array *a, size_t min)
{
    array b = *a;
    if (!min) min = ARRAY_MIN_CAPACITY;
    b.min = min;
    if (array_adjust_capacity (&b)) return 1;
    *a = b;
    return 0;
}

int array_set_capacity_fr (array *a, array_capacity_f fr)
{
    array b = *a;
    if (!fr) fr  = array_capacity_fr;
    b.fr = fr;
    if (array_adjust_capacity (&b)) return 1;
    *a = b;
    return 0;
}

int array_set_capacity_fs (array *a, array_capacity_f fs)
{
    array b = *a;
    if (!fs) fs  = array_capacity_fs;
    b.fs = fs;
    if (array_adjust_capacity (&b)) return 1;
    *a = b;
    return 0;
}

int array_resize (array *a, size_t count)
{
    char *i, *j, *e;

    array b = *a;
    b.count = count;
    if ( ( (count > a->count &&       count  > b.capacity ) ||
           (count < a->count && a->fs(count) < b.capacity )
         ) && array_adjust_capacity(&b) )
        return 1;

    for (i = (char *) b.data + b.count * b.size, e = (char *) b.data + a->count * b.size;
         i < e; i += b.size) /* очищаем изымаемые элементы */
         b.ff(i, b.df);

    for (i = (char *) b.data + a->count * a->size, e = (char *) b.data + b.count * b.size;
         i < e; i += b.size) /* выделяем дополнительные элементы */
    {
        if (b.fa (i, b.da))
        {
            for (j = (char *) b.data + a->count * a->size; j < i; j += b.size)
                b.ff(i, b.df); /* откат, выделение не удалось */
            /* вместимость изменилась, но размер остался прежним */
            return 1;
        }
    }

    *a = b;
    return 0;
}

size_t array_capacity (const array *a)
{
    return a->capacity;
}

size_t array_size (const array *a)
{
    return a->count;
}

int array_set (array *a, size_t i, const void *e)
{
    if (i < a->count)
    {
        /* необходимо очистить существующий элемент
         * в случае неудачи элемент будет уничтожен
         * сделаем попытку вернуть его "на место"
         */
        void *tmp = malloc(a->size);
        if (!tmp) return 1; /* сохранить не удалось */
        if (a->fc (tmp, array_element(a, i), a->dc))
        {
            free(tmp);
            return 1;
        }

        a->ff (array_element(a, i), a->df);
        if (a->fc (array_element(a, i), e, a->dc))
        {
            memcpy (array_element(a, i), tmp, a->size); /* клон уже есть, просто ставим на место */
            free(tmp);
            return 1;
        }
        a->ff(tmp, a->df);
        free(tmp);
        return 0;
    }
    return 1;
}

int array_get (const array *a, size_t i, void *e)
{
    if (i < a->count)
        return a->fc (e, array_element_const(a, i), a->dc);
    return 1;
}

void * array_element (array *a, size_t i)
{
    return (char *) a->data + i * a->size;
}

const void * array_element_const (const array *a, size_t i)
{
    return (char *) a->data + i * a->size;
}

int array_insert (array *a, const void *e)
{
    size_t count = a->count;
    int status = array_resize (a, count + 1);
    if (status) return status;

    if (array_set(a, count, e))
    {   /* неудача копирования */
        if (array_resize(a, count))
        {
            /* возможная неприятность - неудалось вернуть исходный размер
             * просто уменьшим его с сохранением вместимости
             */
            a->ff(array_element(a, count), a->df);
            --a->count;
        }
        return 1;
    }

    return 0;
}

int array_delete (array *a, size_t i)
{
    size_t count = a->count;
    void *tmp = NULL;
    int status;

    if (i >= count) return 1;
    --count;

    if (count > i)
    {
        tmp = malloc(a->size);
        if (!tmp) return 1;
        if (a->fc (tmp, array_element_const(a, count), a->dc))
        {
            free(tmp);
            return 1;
        }
    }
    status = array_resize (a, count);
    if (status)
    {
        a->ff(tmp, a->dc);
        free(tmp);
        return status;
    }

    if (count > i)
    {
        a->ff(array_element(a, i), a->df);
        /* клон уже есть, нет необходимоси клонировать */
        memcpy (array_element(a, i), tmp, a->size);
        free(tmp);
    }
    return 0;
}

void array_foreach (array *a, array_callback_f f, void *data)
{
    char *t = a->data, *e = t + a->count * a->size;
    for (; t < e; t += a->size) f(t, data);
}
void array_foreach_const (const array *a, array_callback_f_const f, void *data)
{
    const char *t = a->data, *e = t + a->count * a->size;
    for (; t < e; t += a->size) f(t, data);
}

void array_iforeach (array *a, array_icallback_f f, void *data)
{
    size_t i = 0;
    for (; i < a->count; ++i) f(array_element(a, i), i, a->count, data);
}
void array_iforeach_const (const array *a, array_icallback_f_const f, void *data)
{
    size_t i = 0;
    for (; i < a->count; ++i) f(array_element_const(a, i), i, a->count, data);
}

