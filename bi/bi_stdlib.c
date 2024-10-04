/*
 * CryptCurio 
 * Copyright (C) 2024 ViMrOd

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    if (b == NULL)
        return;
    free(b->dig);
    b->dig = NULL;
    free(b);
    b = NULL;
}

int BI_copy(BIGINT *ret, const BIGINT *b)
{
    if (ret == b)
        return 1;

    WORD *a = malloc(b->cap * sizeof(*a));
    if (!a)
        return 0;
    memcpy(a, b->dig, b->size * sizeof(*a));
    free(ret->dig);
    ret->dig = a;
    ret->size = b->size;
    ret->cap = b->cap;
    ret->sign = b->sign;

    return 1;
}

int BI_is_zero(const BIGINT *b)
{
    return b->sign == 0 && (b->size == 0);
}

BIGINT* bi_expand(BIGINT *b, const int w)
{
    if (b->cap >= w)
        return b;
    WORD *a = malloc(w * sizeof(*a));
    if (!a)
        return NULL;
    memcpy(a, b->dig, b->size * sizeof(*a));
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

int BI_num_bits(const BIGINT *b)
{
    if (BI_is_zero(b))
        return 0;
    WORD w = b->dig[b->size - 1];
    int c = 0;
    while (w > 0) {
        ++c;
        w >>= 1;
    }
    return c + (b->size - 1) * word_size;
}

WORD BI_is_bit_set(const BIGINT *b, int n)
{
    if (n < 0 || n >= BI_num_bits(b))
        return 0;

    int index = n / word_size;
    int sub_index = n % word_size;
    WORD mask = (WORD)1 << sub_index;
    return b->dig[index] & mask;
}
