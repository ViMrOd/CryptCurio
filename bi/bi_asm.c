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
