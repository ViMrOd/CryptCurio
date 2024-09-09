
#include "bi.h"
#include <assert.h>
#include <stdio.h>

int BI_add(BIGINT *ret, const BIGINT *a, const BIGINT *b)
{
    int r = 0, neg, cmp;

    if (a->sign == b->sign) {
        neg = a->sign;
        r = BI_uadd(ret, a, b);
    } else {
        cmp = BI_ucmp(a, b);

        if (cmp > 0) {
            r = BI_usub(ret, a, b);
            neg = a->sign;
        } else if (cmp < 0) {
            r = BI_usub(ret, b, a);
            neg = b->sign;
        }
        else {
            neg = 0;
            BI_zero(ret);
            r = 1;
        }
    }

    ret->sign = neg;
    return r;
}

int BI_sub(BIGINT *ret, const BIGINT *a, const BIGINT *b)
{
    int r = 0, neg, cmp;

    if (a->sign != b->sign) {
        neg = a->sign;
        r = BI_uadd(ret, a, b);
    } else {
        cmp = BI_ucmp(a, b);
        if (cmp > 0) {
            neg = a->sign;
            r = BI_usub(ret, a, b);
        } else if (cmp < 0) {
            neg = !a->sign;
            r = BI_usub(ret, b, a);
        } else {
            neg = 0;
            BI_zero(ret);
            r = 1;
        }
    }

    ret->sign = neg;
    return r;
}

/* sets ret to unsigned addition of a + b */
int BI_uadd(BIGINT *ret, const BIGINT *a, const BIGINT *b)
{
    /* 
     * Want a to be larger than b so the add_words function works as expected
     * The swap is only valid inside of BI_uadd and not in the outside program
     */
    if (a->size < b->size) {
        const BIGINT *tmp;
        tmp = a;
        a = b;
        b = tmp;
    }

    const WORD *ad = a->dig;
    const WORD *bd = b->dig;

    int max = a->size;
    int min = b->size;
    int diff = max - min;

    ret = bi_expand(ret, max + 1);
    WORD *rd = ret->dig;
    ret->size = max;

    WORD carry = bi_add_words(rd, ad, bd, min);
    ad += min;
    rd += min;

    while (diff--) {
        WORD t1 = ad[0] + carry;
        carry = carry && t1 == 0;
        rd[0] = t1;
        ++ad;
        ++rd;
    }

    rd[0] = carry;
    ret->size += carry;

    BI_correct_top(ret);

    return 1;
}

/*
 * Sets ret to the unsigned subtraction of a - b
 * In other words ret = |a| - |b|
 * Undefined behavior occurs if |a| < |b| : Here it just fails an assertion
 */
int BI_usub(BIGINT *ret, const BIGINT *a, const BIGINT *b)
{
    const WORD *ad = a->dig;
    const WORD *bd = b->dig;

    /* No swap here like BI_uadd as the caller must ensure that |a| >= |b| */
    int max = a->size;
    int min = b->size;
    int diff = max - min;

    assert(diff >= 0 && "|a| < |b|");

    ret = bi_expand(ret, max);
    WORD *rd = ret->dig;
    ret->size = max;

    WORD borrow = bi_sub_words(rd, ad, bd, min);
    ad += min;
    rd += min;

    while (diff--) {
        WORD t1 = ad[0] - borrow;
        borrow = borrow && ad[0] == 0;
        rd[0] = t1; 
        ++ad;
        ++rd;
    }    

    while (ret->size && (--rd[0]) == 0)
        --ret->size;

    ret->sign = 0;

    return 1;
}
