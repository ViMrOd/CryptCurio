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

#include <string.h>

#include "bi.h"

int BI_lshift1(BIGINT *r, const BIGINT *a)
{
    const WORD *ad;
    WORD *rd, t, carry = 0;
    int i;

    r->sign = a->sign;
    if (bi_expand(r, a->size + 1) == 0)
        return 0;
    r->size = a->size;

    ad = a->dig;
    rd = r->dig;
    for (i = 0; i < a->size; i++) {
        t = (ad++)[0];
        (rd++)[0] = (t << 1) | carry;
        carry = t >> (word_size - 1);
    }
    rd[0] = carry;
    r->size += carry;
    //BI_correct_top(r);
    return 1;
}

int BI_lshift(BIGINT *r, const BIGINT *a, int n)
{
    BI_copy(r, a);
    for (int i = 0; i < n; i++) {
        if (BI_lshift1(r, r) == 0)
            return 0;
    }
    return 1;
}

int BI_rshift1(BIGINT *r, const BIGINT *a)
{
    const WORD *ad;
    WORD *rd, t, carry = 0;
    int i;

    r->sign = a->sign;
    if (bi_expand(r, a->size) == 0)
        return 0;
    r->size = a->size;

    ad = a->dig;
    rd = r->dig;
    for (i = 0; i < a->size - 1; i++) {
        t = ad[i];
        carry = ad[i + 1] & 1;
        rd[i] = (t >> 1) | (carry << (word_size - 1));
    }
    rd[a->size - 1] = ad[a->size - 1] >> 1;

    BI_correct_top(r);
    if (r->size == 0)
        r->sign = 0;
    return 1;
}

int BI_rshift(BIGINT *r, const BIGINT *a, int n)
{
    BI_copy(r, a);
    for (int i = 0; i < n; i++) {
        if (BI_rshift1(r, r) == 0)
            return 0;
    }
    return 1;
}
