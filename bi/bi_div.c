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

#include <assert.h>

#include "bi.h"

//a=(q*b)+r
#if 1
int BI_div(BIGINT *q, BIGINT *r, const BIGINT *a, const BIGINT *b)
{
    assert(b->size > 0 && "Division by zero error");

    if (BI_is_zero(a)) {
        if (q != 0)
            BI_zero(q);
        if (r != 0)
            BI_zero(r);
    }

    int free_q = 0, free_r = 0;

    if (BI_ucmp(a, b) < 0) {
        if (q != 0)
            BI_zero(q);
        if (r != 0) {
            if (!BI_copy(r, a))
                return 0;
        }
        return 1;
    }

    int ret = 0;

    BIGINT *B = BI_new();
    if (q == 0) {
        q = BI_new();
        free_q = 1;
    }
    if (r == 0) {
        r = BI_new();
        free_r = 1;
    }
    if (B == 0 || q == 0 || r == 0)
        goto end;

    int na = BI_num_bits(a);
    int nb = BI_num_bits(b);
    int a_sign = a->sign;
    int b_sign = b->sign;
    if (!(BI_copy(B, b) && BI_copy(r, a)))
        goto end; 
    BI_zero(q);
    if (bi_expand(q, 1) == 0)
        goto end;
    q->dig[0] = 0;

    if (BI_lshift(B, B, na - nb) == 0)
        goto end;
    for (int i = na - nb; i >= 0; i--) {
        if (BI_lshift1(q, q) == 0)
            goto end;
        if (BI_ucmp(r, B) >= 0) {
            q->dig[0] |= 1;
            if (q->size == 0)
                q->size = 1;
            if (BI_usub(r, r, B) == 0)
                goto end;
        }
        if (BI_rshift1(B, B) == 0)
            goto end;
    }
    r->sign = BI_is_zero(r) ? 0 : a_sign;
    q->sign = a_sign ^ b_sign;

    BI_correct_top(q);
    BI_correct_top(r);

    ret = 1;
end:
    BI_free(B);
    if (free_q)
        BI_free(q);
    if (free_r)
        BI_free(r);

    return ret;
}
#else 
int BI_div(BIGINT *q, BIGINT *r, const BIGINT *x, const BIGINT *y)
{
    assert(y->size > 0 && "Division by zero error");

    if (BI_is_zero(x)) {
        if (q != 0)
            BI_zero(q);
        if (r != 0)
            BI_zero(r);
    }

    int free_q = 0, free_r = 0;

    if (BI_ucmp(x, y) < 0) {
        if (q != 0)
            BI_zero(q);
        if (r != 0) {
            if (!BI_copy(r, a))
                return 0;
        }
        return 1;
    }

    int ret = 0;

    BIGINT *B = BI_new();
    if (q == 0) {
        q = BI_new();
        free_q = 1;
    }
    if (r == 0) {
        r = BI_new();
        free_r = 1;
    }
    if (B == 0 || q == 0 || r == 0)
        goto end;

    int nx = x->size;
    int ny = y->size;
    int x_sign = x->sign;
    int y_sign = y->sign;

    if (!(BI_copy(B, y) && BI_copy(r, x)))
        goto end; 
    
    if (bi_expand(q, nx - ny + 1) == 0)
        goto end;

    BIGINT *ctx = BI_new();
    BI_copy(ctx, y);
    BI_lshift(ctx, ctx, word_size * (nx - ny));

    while (BI_ucmp(x, ctx) >= 0) {
        BI_usub(ctx, x, ctx);
    }

    for (int i = na - nb; i >= 0; i--) {
        if (BI_lshift1(q, q) == 0)
            goto end;
        if (BI_ucmp(r, B) >= 0) {
            q->dig[0] |= 1;
            if (q->size == 0)
                q->size = 1;
            if (BI_usub(r, r, B) == 0)
                goto end;
        }
        if (BI_rshift1(B, B) == 0)
            goto end;
    }
    r->sign = BI_is_zero(r) ? 0 : a_sign;
    q->sign = a_sign ^ b_sign;

    BI_correct_top(q);
    BI_correct_top(r);

    ret = 1;
end:
    BI_free(ctx);
    BI_free(B);
    if (free_q)
        BI_free(q);
    if (free_r)
        BI_free(r);

    return ret;
}
#endif
