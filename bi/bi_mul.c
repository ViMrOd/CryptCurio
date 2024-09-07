
#include "bi.h"

int BI_mul(BIGINT *ret, const BIGINT *a, const BIGINT *b)
{
    int na = a->size;
    int nb = b->size;
    int nr = na + nb;

    BIGINT *r;

    /* 
     * Need a temporary BIGNUM context since multiplication unlike addition
     * and subtraction requires going back to previous limbs in a and b so 
     * those must be saved 
     */
    if (ret == a || ret == b)
        r = BI_new();
    else
        r = ret;

    r->sign = a->sign ^ b->sign;
    r = bi_expand(r, nr);
    r->size = nr;
    bi_mul(r->dig, a->dig, na, b->dig, nb);

    BI_copy(ret, r);
    BI_free(r);

    return 1;
}

void bi_mul(WORD *ret, const WORD *a, int na, const WORD *b, int nb)
{   
    /* rr is not a seperate array from ret, it is the same array just shifted */
    WORD *rr = ret + na;

    /* 
     * rr[0] is just the carry from multiplying a * b[0] 
     * Then ret is the first intermediate product
     */
    rr[0] = bi_mul_words(ret, a, na, b[0]);

    /* 
     * This is gradeschool multiplication with shifting the intermediate 
     * products a * b[i] and then adding all the intermediate products 
     * together at the end to get the final product a * b 
     * rr is shifed by 1 to because each subsequent intermediate product is 
     * shifted to the right
     * ret is shifted because of the implicit zeros added when shifing a number 
     * will not change that place when summing the intermediate products 
     * together
     */
    while (--nb) {
        rr[1] = bi_mul_add_words(&ret[1], a, na, b[1]);
        ++rr;
        ++ret;
        ++b;
    }
}
