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

/*
 * Contains low level implemations for aritmetic functions
 * Not meant to use in user programs.
 */

#include "bi.h"
#include <assert.h>

WORD bi_add_words(WORD *ret, const WORD *a, const WORD *b, int n) 
{
    WORD carry = 0, t1, t2;

    while (n--) {
        t1 = a[0];
        t1 += carry;
        carry = carry && t1 == 0;

        t2 = b[0];
        t1 += t2;
        carry = carry || (t1 < t2);

        ret[0] = t1;

        ++a;
        ++b;
        ++ret;
    }

    return carry;
}

WORD bi_mul_add_words(WORD *ret, const WORD *a, int n, WORD b)
{
    WORD carry = 0;
    while (n--) {
        mul_add(ret[0], a[0], b, carry);
        ++a;
        ++ret;
    }
    return carry;
}

WORD bi_mul_words(WORD *ret, const WORD *a, int n, WORD b)
{
    WORD carry = 0;
    while (n--) {
        mul(ret[0], a[0], b, carry);
        ++a;
        ++ret;
    }
    return carry;
}

WORD bi_sub_words(WORD *ret, const WORD *a, const WORD *b, int n)
{
    WORD t1, t2, borrow = 0;

    while (n--) {
        t1 = a[0];
        t2 = t1 - borrow;
        borrow = borrow && a[0] == 0;

        t1 = b[0];
        t1 = t2 - t1;
        borrow = borrow || t1 > t2;

        ret[0] = t1;

        ++a;
        ++b;
        ++ret;
    }

    return borrow;
}
