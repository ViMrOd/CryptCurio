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
 * This demo uses the Miller-Rabin Algorithm to generate b bit psuedo
 * random prime numbers.
 * By default b is set to 1536 bits.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "bi.h"

int miller_rabin(const BIGINT *n, int k)
{
    assert((n->dig[0] & 1));

    BIGINT *a = BI_new();
    BIGINT *d = BI_new();
    BIGINT *one = BI_new(); hex_to_bi(one, "1");
    BIGINT *two = BI_new(); hex_to_bi(two, "2");
    BIGINT *three = BI_new(); hex_to_bi(three, "3");
    BIGINT *n_3 = BI_new(); BI_sub(n_3, n, three);
    BIGINT *x = BI_new();
    BIGINT *n_1 = BI_new(); BI_sub(n_1, n, one);

    BI_sub(d, n, one);
    int s = 0;
    while ((d->dig[0] & 1) == 0) {
        BI_rshift1(d, d);
        ++s;
    }

    int ret = 0;
    for (int i = 0; i < k; i++) {
        BI_rand_range(a, n_3, 0, 0); //[0, n-4]
        BI_add(a, a, two); //[2, n-2]
        BI_modpow(x, a, d, n);
        if (BI_cmp(x, one) == 0 || BI_cmp(x, n_1) == 0) //a^d % n = 1
            continue;

        int cont = 0;
        for (int j = 1; j < s; j++) {
            BI_modpow(x, x, two, n); //x^2 % n = -1
            if (BI_cmp(x, n_1) == 0) {
                cont = 1;
                break;
            }      
        }
        if (cont == 0)
            goto end;
    }
    ret = 1;
end:
    BI_free(n_1);
    BI_free(n_3);
    BI_free(x);
    BI_free(three);
    BI_free(two);
    BI_free(one);
    BI_free(d);
    BI_free(a);
    return ret;
}

int main(int argc, char **argv)
{
    int b, k;
    if (argc == 2)
      b = atoi(argv[1]);
    else
        b = 1536;

    if (b >= 3747)
        k = 3;
    else if (b >= 1345)
        k = 4;
    else if (b >= 476)
        k = 5;
    else if (b >= 400)
        k = 6;
    else if (b >= 347)
        k = 7;
    else if (b >= 308)
        k = 8;
    else if (b >= 55)
        k = 27;
    else
        k = 34;

    BIGINT *n = BI_new(), *two = BI_new();
    BI_rand(n, b, 1, 1);
    hex_to_bi(two, "2");
    int res, it = 0, skip = 0;
    
    BIGINT *p = BI_new(), *r = BI_new();
    WORD primes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

    do {
#if 1
        for (size_t i = 0; i < (sizeof(primes) / sizeof(primes[0])); i++) {
            p->dig[0] = primes[i]; p->size = 1;
            if (BI_rem(r, n, p) == 0)
                return 1;
            if (BI_is_zero(r)) {
                ++skip;
                goto next;
            }
        }
#endif
        res = miller_rabin(n, k);
next:
        BI_add(n, n, two);
        ++it;
    } while (res == 0);

    BI_sub(n, n, two);

    //assert(miller_rabin(n, k) == 1);
    printf("Found %d bit probable prime::\n", BI_num_bits(n));
    printf("%s\n", bi_to_hex(n));
    printf("Took %d iterations; Skipped %d through trial division\n", it, skip);

    BI_free(n);
    BI_free(two);
    BI_free(p);
    BI_free(r);

    return 0;
}
