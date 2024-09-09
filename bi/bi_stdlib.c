
#include "bi.h"
#include <stdlib.h>
#include <string.h>

BIGINT* BI_new(void)
{
    BIGINT *b = malloc(sizeof(BIGINT));
    if (b  == NULL)
        return NULL;
    b->size = 0;
    b->cap = 1;
    b->sign = 0;
    b->dig = malloc(word_size);
    if (b->dig  == NULL) {
        free(b);
        b = NULL;
        return NULL;
    }
    return b;
}

void BI_free(BIGINT *b)
{
    free(b->dig);
    b->dig = NULL;
    free(b);
    b = NULL;
}

void BI_copy(BIGINT *ret, const BIGINT *a)
{
    ret->size = a->size;
    ret->cap = a->cap;
    ret->sign = a->sign;
    //clear ret->dig and copy a->dig to ret->dig
}

//Bug in this function
int BI_is_zero(const BIGINT *b)
{
    return b->dig[0] == 0 && (b->size == 0);
}

BIGINT* bi_expand(BIGINT *b, const int w)
{
    if (b->cap >= w)
        return b;
    WORD *a = malloc(w * sizeof(*a));
    if (!a)
        return NULL;
    if (b->size > 0)
        memcpy(a, b->dig, b->size * sizeof(*a));
    if (b->dig != NULL)
        free(b->dig);
    b->dig = a;
    b->cap = w;
    return b;
}

void BI_correct_top(BIGINT *b)
{
    int z = 0;
    for (int i = b->size - 1; i >= 0; i--) {
        if (b->dig[i] != 0)
            break;
        z++;
    }
    b->size -= z;
}

void BI_zero(BIGINT *a)
{
    a->sign = 0;
    a->size = 0;
}
