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

int BI_cmp(const BIGINT *a, const BIGINT *b)
{
    if (a->sign != b->sign) {
        if (a->sign > b->sign) { //1 0
            return -1;
        } else {
            assert(b->sign > a->sign); //0 1
            return 1;
        }
    }
    else if (a->sign == 1) { //1 1
        assert(b->sign == 1);
        return BI_ucmp(b, a);
    } else {
        assert(a->sign == 0 && b->sign == 0); //0 0
        return BI_ucmp(a, b);
    }
}

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
