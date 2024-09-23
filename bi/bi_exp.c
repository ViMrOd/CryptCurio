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
