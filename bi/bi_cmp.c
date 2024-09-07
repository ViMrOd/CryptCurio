
#include "bi.h"

int BI_ucmp(const BIGINT *a, const BIGINT *b)
{
    int i;

    i = a->size - b->size;
    if (i != 0)
        return i;

    for (i = a->size - 1; i >= 0; i--) {
        if (a->dig[i] != b->dig[i])
            return a->dig[i] > b->dig[i] ? 1 : -1;
    }

    return 0;
}
