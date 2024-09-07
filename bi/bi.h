/*
 *
 *
 *
 *
 */

#ifndef BI_H
#define BI_H

typedef unsigned long WORD;
#define MASK (0xffffffffffffffff)

#define word_size (sizeof(WORD) * 8)
#define system_size (64)

typedef struct BI_STRUCT
{
    int size;
    int cap;
    WORD *dig;
    int sign;
} BIGINT;

/*
 * Since my system is 64 bits and my compiler supports 128-bit integers, it
 * makes sense for me to utilize 128-bit integers for multiplication. This
 * choice is preferable to performing 4 N/2 multiplications with additional
 * shifts.
 * 
 * The intermediate product, being 128 bits, is then split into high and low
 * 64-bit words. This splitting process resembles how architectures that
 * support storing the high word in a general-purpose register handle data.
 * 
 * Subsequently, the 64-bit low word is added to the carry, with the carry
 * consisting of the high word plus any overflow from the initial addition.
 * 
 * One might question why the addition isn't performed with 128-bit integers
 * to eliminate the need for carry detection. My working theory is that
 * performing 64-bit operations natively is faster for the chip than performing
 * 128-bit operations, and this speed advantage outweighs the overhead of carry
 * detection. This theory is backed up by some quick and dirty benchmarking I 
 * performed, although this theory definetly requires more testing.
 * 
 * This rationale aligns with the fact that addition, subtraction, and division 
 * typically don't involve 128-bit integers. An exception to this rule occurs 
 * in 32-bit systems using 64-bit integers (BN_LLONG) for intermediate results 
 * in multiplication, addition, and division.
 * 
 * My theory regarding this exception is that these chips that support BN_LLONG 
 * possess native instructions to handle these 64-bit words, thereby achieving 
 * nearly native speeds with 64-bit integers as with 32-bit integers. Once 
 * again this theory requires some benchmarking to confirm.
 *
 * If my theory is correct, then if in the future instructions are added to 
 * natively handle 128 bit integers, then it would be faster to perform all 
 * intermediate operations with 128 bit intgers similar to BN_LLONG.
 */

/*
 *__128_BIT macro can be undefined to show how to do multiplication on a system 
 * that does not support 2*N integers in the future.
 */
#define __128_BIT
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16 && defined(__128_BIT)
/*
 * The u_mul_lohi macro allows us to multiply two limbs together and store the 
 * top half of the product (most significant bits) in __high and the bottom 
 * half (least significant bits) in __low.
 * The use of double limbs allows for the use of only one multiplication and 
 * thus more efficient implementation.
 */
#define u_mul_lohi(__low, __high, __a, __b)               \
        do {                                              \
            __uint128_t uv = (__uint128_t) (__a) * (__b); \
            (__low) = uv & MASK;                          \
            (__high) = uv >> 64;                          \
        } while (0)

#define mul(__ret, __a, __b, __carry)            \
        do {                                     \
            WORD low, high;                      \
            u_mul_lohi(low, high, (__a), (__b)); \
            (__ret) = low + (__carry);           \
            (__carry) = high + ((__ret) < low);  \
        } while (0)

#define mul_add(__ret, __a, __b, __carry)        \
        do {                                     \
            WORD low, high;                      \
            u_mul_lohi(low, high, (__a), (__b)); \
            (__ret) += (__carry);                \
            (__carry) = ((__ret) < (__carry));   \
            (__carry) += high;                   \
            (__ret) += low;                      \
            (__carry) += ((__ret) < low);        \
        } while (0)
#else
/* 
 * Possible future implementation of non double limb multiplication goes here ;) 
 * This would be a huge performance hit as more multiplications and additions
 * would be required.
 */
#endif /* 128 BIT */

WORD bi_add_words(WORD *ret, const WORD *a, const WORD *b, int n);
WORD bi_sub_words(WORD *ret, const WORD *a, const WORD *b, int n);
WORD bi_mul_words(WORD *rop, const WORD *a, int n, WORD b);
WORD bi_mul_add_words(WORD *rop, const WORD *a, int n, WORD b);

/******************************************************************************/

BIGINT* BI_new(void);
void BI_free(BIGINT *b);
void BI_copy(BIGINT *ret, const BIGINT *a);

BIGINT* bi_expand(BIGINT *b, const int w);

void hex2bi(BIGINT *b, const char *s);
char* bn2hex(const BIGINT *b);

int BI_add(BIGINT *ret, const BIGINT *a, const BIGINT *b);
int BI_sub(BIGINT *ret, const BIGINT *a, const BIGINT *b);
int BI_uadd(BIGINT*, const BIGINT*, const BIGINT*);
int BI_usub(BIGINT*, const BIGINT*, const BIGINT*);

void bi_mul(WORD *ret, const WORD *a, int na, const WORD *b, int nb);
int BI_mul(BIGINT *ret, const BIGINT *a, const BIGINT *b);

int BI_ucmp(const BIGINT *a, const BIGINT *b);

int BI_is_zero(const BIGINT*);
void BI_zero(BIGINT *a);

#endif /* BI_H */
