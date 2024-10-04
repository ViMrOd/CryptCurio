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

#if 1
int BI_pow(BIGINT *r, const BIGINT *a, const BIGINT *b)
{
    int t = 0, free_r = 0;

    BIGINT *ret;
    if (r == a || r == b) {
        ret = BI_new();
        free_r = 1;
        if (!ret)
            goto end;
    } else {
        ret = r;
    }

    hex_to_bi(ret, "1");

    int n = BI_num_bits(b);
    for (int i = n - 1; i >= 0; i--) {
        if (!BI_sqr(ret, ret))
            goto end;
        if (BI_is_bit_set(b, i)) {
            if (BI_mul(ret, ret, a) == 0)
                goto end;
        }
    }
    BI_copy(r, ret);
    BI_correct_top(r);

    t = 1;
end:
    if (free_r == 1)
        BI_free(ret);
    return t;
}
#else
//testing sliding window method
#define K 1

int BI_pow(BIGINT *r, const BIGINT *a, const BIGINT *n)
{
    int t = 0;
    BIGINT *res;
    int free_r = 0;
    if (r == a || r == n) {
        res = BI_new();
        free_r = 1;
    } else {
        res = r;
    }

    if (!BI_sqr(res, a))
        goto end;

    int i, j;

    BIGINT *arr[1 << (K - 1)];
    for (i = 0; i < 1 << (K - 1); i++) {
        arr[i] = BI_new();
        if (!arr[i])
            goto end; //causes bug cuz free unallocated memory
    }

    if (!BI_copy(arr[0], a))
        goto end;
    for (i = 1; i < (1 << (K - 1)); i++) {
        if (!BI_mul(arr[i], arr[i - 1], res))
            goto end;
    }

    hex_to_bi(res, "1");

    i = BI_num_bits(n) - 1;
    while (i >= 0) {
        if (BI_is_bit_set(n, i)) {
            int win = i + 1 >= K ? K : i + 1;
            int l = i;
            int ind = 1;
            for (j = 1; j < win; j++) {
                ind = ind << 1;
                if (BI_is_bit_set(n, i - j)) {
                    l = i - j;
                    ind |= 1;
                }
            }
            while ((ind & 1) == 0)
                ind = ind >> 1;
            for (j = 0; j < (i - l + 1); j++) {
                if (!BI_sqr(res, res))
                    goto end;
            }
            if (!BI_mul(res, res, arr[ind/2]))
                goto end;
            i = l - 1;
        } else {
            if (!BI_sqr(res, res))
                goto end;
            i--;
        }
    }

    if (!BI_copy(r, res))
        goto end;
    t = 1;
end:
    for (i = 0; i < 1 << (K - 1); i++)
        BI_free(arr[i]);
    if (free_r)
        BI_free(res);

    return t;
}
#endif

