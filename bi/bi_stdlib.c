
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

int BI_is_zero(const BIGINT *b)
{
    return b->dig[0] == 0 && (b->size == 0);
}

BIGINT* bi_expand(BIGINT *b, const int w)
{
    if (b->cap >= w)
        return b;
    b->dig = realloc(b->dig, w * sizeof(*b->dig));
    if (b->dig == NULL)
        return NULL;
    return b;
}

void BI_zero(BIGINT *a)
{
    a->sign = 0;
    a->size = 0;
}
