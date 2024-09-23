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
#include <assert.h>

int BI_mod(BIGINT *r, const BIGINT *a, const BIGINT *m)
{
    /* 
     * Why??? 
     * Like I know m is used later but is the assert the only way?
     * Limit the arguments for all functions that use this function
     */
    if (r == m)
        assert(0 && "Invalid Args");
    
    if (BI_rem(r, a, m) == 0)
        return 0;
    if (r->sign == 0)
        return 1;
    return (m->sign == 1 ? BI_sub : BI_add) (r, r, m);
}

int BI_modmul(BIGINT *r, const BIGINT *a, const BIGINT *b, const BIGINT *m)
{
    int ret = 0;
    if (!BI_mul(r, a, b) || !BI_mod(r, r, m))
        goto end;
    BI_correct_top(r);
    ret = 1;
end:
    return ret;
}

int BI_modpow(BIGINT *r, const BIGINT *a, const BIGINT *b, const BIGINT *m)
{
    int t = 0, clean = 0;

    BIGINT *ret;
    if (r == a || r == b) {
        ret = BI_new();
        clean = 1;
    } else
        ret = r;
    hex_to_bi(ret, "1");

    int n = BI_num_bits(b);
    for (int i = n - 1; i >= 0; i--) {
        if (BI_sqr(ret, ret) == 0)
            goto end;
        BI_mod(ret, ret, m);
        if (BI_is_bit_set(b, i)) {
            if (!BI_modmul(ret, ret, a, m))
                goto end;
        }
    }
    BI_copy(r, ret);
    BI_correct_top(r);

    t = 1;
end:
    if (clean)
        BI_free(ret);
    return t;
}
