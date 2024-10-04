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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bi.h"

int get_bi(BIGINT *b, const char *op, const char *line)
{
    int i = strlen(op) + 3; /* + 3 comes from 3 chars in " = " */
    char l[4096];
    strcpy(l, &line[i]);
    l[strlen(l) - 1] = '\0'; /* Strip newline char */
    hex_to_bi(b, l);
    if(strcmp(bi_to_hex(b), l) != 0) {
        printf("%s\n%s\n", l, bi_to_hex(b));
        return 0;
    }
    return 1;
}

#define GET_BI(__a, __b) get_bi(__a, #__a, __b)

#define ERR(__msg) fprintf(stderr, "[ERROR] %s Exiting with error\n", __msg)

int file_test_sum(void)
{
    FILE *fp = fopen("recipies/bi/bi_add.txt", "r");
    if (!fp) {
        ERR("Could not open file \"bi_add.txt\"");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *B, *Sum, *Res;
    A = BI_new();
    B = BI_new();
    Sum = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("Sum", buff, 3) == 0) {
            GET_BI(Sum, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'B') {
            GET_BI(B, buff);

            /*
             * Beginning of actual test cases. I do not like how this is 
             * currently implemented and would like to clean this up and
             * refactor, thus the actual testing will not be done inside this 
             * block and provide better error messages upon test failure. How 
             * that will be done I still do not know yet. I also am not big fan
             * of the asserts and would like to work on that in the future. I 
             * want a method with better error messages and jump to cleanup. 
             * But for the moment, these tests get the job done.
             */
            BI_add(Res, A, B); //A+B
            assert(BI_cmp(Res, Sum) == 0);
            BI_sub(Res, Sum, A); //Sum-A
            assert(BI_cmp(Res, B) == 0);
            BI_sub(Res, Sum, B); //Sum-B
            assert(BI_cmp(Res, A) == 0);
            
            /* 
             * Same tests as above but Res points to A or Res points to B 
             */
            BI_copy(Res, A);
            BI_add(Res, Res, B); //A+B
            assert(BI_cmp(Res, Sum) == 0);
            BI_copy(Res, B);
            BI_add(Res, A, Res); //A+B
            assert(BI_cmp(Res, Sum) == 0);
            BI_copy(Res, Sum);
            BI_sub(Res, Res, A); //Sum-A
            assert(BI_cmp(Res, B) == 0);
            BI_copy(Res, A);
            BI_sub(Res, Sum, Res); //Sum-A
            assert(BI_cmp(Res, B) == 0);
            BI_copy(Res, Sum);
            BI_sub(Res, Res, B); //Sum-B
            assert(BI_cmp(Res, A) == 0);
            BI_copy(Res, B);
            BI_sub(Res, Sum, Res); //Sum-B
            assert(BI_cmp(Res, A) == 0);

            /* 
             * These are just the same tests as above but with unsigned. 
             * variant. These tests assume that the prereqresuites all hold.
             * This may be fleshed out in the future to test conditions where 
             * the prereqresuites may not hold.
             */ 
            if (A->sign == 0 && B->sign == 0 && BI_cmp(A, B) >= 0) {
                BI_uadd(Res, A, B); //A+B
                assert(BI_cmp(Res, Sum) == 0);
                BI_usub(Res, Sum, A); //Sum-A
                assert(BI_cmp(Res, B) == 0);
                BI_usub(Res, Sum, B); //Sum-B
                assert(BI_cmp(Res, A) == 0);

                BI_copy(Res, A);
                BI_uadd(Res, Res, B); //A+B
                assert(BI_cmp(Res, Sum) == 0);
                BI_copy(Res, B);
                BI_uadd(Res, A, Res); //A+B
                assert(BI_cmp(Res, Sum) == 0);
                BI_copy(Res, Sum);
                BI_usub(Res, Res, A); //Sum-A
                assert(BI_cmp(Res, B) == 0);
                BI_copy(Res, A);
                BI_usub(Res, Sum, Res); //Sum-A
                assert(BI_cmp(Res, B) == 0);
                BI_copy(Res, Sum);
                BI_usub(Res, Res, B); //Sum-B
                assert(BI_cmp(Res, A) == 0);
                BI_copy(Res, B);
                BI_usub(Res, Sum, Res); //Sum-B
                assert(BI_cmp(Res, A) == 0);
            }

            /*
             * BI_add_word and BI_sub_word go here once implemented.
             */

            continue;
        }
        ERR("Unexpected line in test file");
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(B);
    BI_free(Sum);
    BI_free(Res);
    return ret;
}

int file_test_mul(void)
{
    FILE *fp = fopen("recipies/bi/bi_mul.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_mul.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *B, *Product, *Res;
    A = BI_new();
    B = BI_new();
    Product = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("Product = ", buff, 10) == 0) {
            if (!GET_BI(Product, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'A') {
            if (!GET_BI(A, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'B') {
            if (!GET_BI(B, buff))
                goto cleanup;

            if (!BI_mul(Res, A, B) || BI_cmp(Res, Product))
                goto cleanup;

            if (!BI_copy(Res, A) || !BI_mul(Res, Res, B) || BI_cmp(Res, Product))
                goto cleanup;

            if (!BI_copy(Res, B) || !BI_mul(Res, A, Res) || BI_cmp(Res, Product))
                goto cleanup;

            continue;
        }
        ERR("Unexpected line in file");
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(B);
    BI_free(Product);
    BI_free(Res);
    return ret;
}

int file_test_sqr(void)
{
    FILE *fp = fopen("recipies/bi/bi_sqr.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_sqr.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *Square, *Res;
    A = BI_new();
    Square = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("Square = ", buff, 9) == 0) {
            if (!GET_BI(Square, buff))
                goto cleanup;
            continue;
        }

        if (buff[0] == 'A') {
            if (!GET_BI(A, buff))
                goto cleanup;

            if (!BI_sqr(Res, A) || BI_cmp(Res, Square))
                goto cleanup;

            if (!BI_copy(Res, A) || !BI_sqr(Res, Res) || BI_cmp(Res, Square))
                goto cleanup;

            continue;
        }
        ERR("Unexpected line in file");
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(Square);
    BI_free(Res);
    return ret;
}

int test_lshift1_file(void)
{
    FILE *fp = fopen("recipies/bi/bi_lshift1.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_lshift1.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *LShift1, *Two, *Res, *Remainder;
    A = BI_new();
    LShift1 = BI_new();
    Res = BI_new();
    Two = BI_new();
    Remainder = BI_new();

    hex_to_bi(Two, "2");

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("LShift1 = ", buff, 10) == 0) {
            GET_BI(LShift1, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);

            BI_add(Res, A, A); //A+A
            assert(BI_cmp(Res, LShift1) == 0);
            BI_mul(Res, A, Two); //A*2
            assert(BI_cmp(Res, LShift1) == 0);
            BI_div(Res, Remainder, LShift1, Two); //Res=LShift/2
            assert(BI_cmp(Res, A) == 0);
            assert(BI_is_zero(Remainder));
            BI_lshift1(Res, A); //Res=A<<1
            assert(BI_cmp(Res, LShift1) == 0);
            BI_rshift1(Res, LShift1);
            assert((BI_cmp(Res, A) == 0));

            continue;
        }

        ERR("Unexpected line in file");
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(LShift1);
    BI_free(Two);
    BI_free(Res);
    BI_free(Remainder);
    return ret;
}

int test_lshift_file(void)
{
    FILE *fp = fopen("recipies/bi/bi_lshift.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_lshift.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *LShift, *Res;
    int n;
    A = BI_new();
    LShift = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("LShift = ", buff, 9) == 0) {
            GET_BI(LShift, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'N') {
            char *p = &buff[4], *end;
            n = strtol(p, &end, 16);

            BI_lshift(Res, A, n); //A<<n
            assert(BI_cmp(LShift, Res) == 0);
            continue;
        }

        ERR("Unexpected line in file");
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(LShift);
    BI_free(Res);
    return ret;
}

int test_rshift_file(void)
{
    FILE *fp = fopen("recipies/bi/bi_rshift.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_rshift.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *RShift, *Res;
    int n;
    A = BI_new();
    RShift = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("RShift = ", buff, 9) == 0) {
            GET_BI(RShift, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'N') {
            char *p = &buff[4], *end;
            n = strtol(p, &end, 16);

            BI_rshift(Res, A, n); //A<<n
            assert(BI_cmp(RShift, Res) == 0);

            if (n == 1) {
                BI_rshift1(Res, A);
                assert(BI_cmp(RShift, Res) == 0);
            }

            continue;
        }

        ERR("Unexpected line in file");
        fprintf(stderr, "%s\n", buff);
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(RShift);
    BI_free(Res);
    return ret;
}

int test_div_file(void)
{
    FILE *fp = fopen("recipies/bi/bi_div.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_div.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *B, *Quotient, *Remainder, *Res1, *Res2;
    int n;
    A = BI_new();
    B = BI_new();
    Quotient = BI_new();
    Remainder = BI_new();
    Res1 = BI_new();
    Res2 = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("Quotient = ", buff, 11) == 0) {
            if (!GET_BI(Quotient, buff))
                goto cleanup;
            continue;
        }
        if (strncmp("Remainder = ", buff, 12) == 0) {
            if (!GET_BI(Remainder, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'A') {
            if (!GET_BI(A, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'B') {
            if (!GET_BI(B, buff))
                goto cleanup;

            if (!BI_div(Res1, Res2, A, B))
                goto cleanup;
            if (BI_cmp(Res1, Quotient) || BI_cmp(Res2, Remainder)) {
                fprintf(stderr, "Expected :: %s\nGot :: %s\n", bi_to_hex(Quotient), bi_to_hex(Res1));
                fprintf(stderr, "Expected :: %s\nGot :: %s\n\n", bi_to_hex(Remainder), bi_to_hex(Res2));
                goto cleanup;
            }

            if (!BI_mul(Res2, Quotient, B) || !BI_add(Res2, Res2, Remainder))
                goto cleanup;
            if (BI_cmp(Res2, A)) {
                fprintf(stderr, "Expected :: %s\nGot :: %s\n\n", bi_to_hex(A), bi_to_hex(Res2));
                goto cleanup;
            }
            
            if (!BI_copy(Res2, A) || !BI_div(Res1, Res2, Res2, B))
                goto cleanup;
            if (BI_cmp(Res1, Quotient) || BI_cmp(Res2, Remainder)) {
                fprintf(stderr, "ERROR :: Rem == A\n");
                fprintf(stderr, "A :: %s\nB :: %s\n", bi_to_hex(A), bi_to_hex(B));
                fprintf(stderr, "Expected :: %s\nGot :: %s\n", bi_to_hex(Quotient), bi_to_hex(Res1));
                fprintf(stderr, "Expected :: %s\nGot :: %s\n\n", bi_to_hex(Remainder), bi_to_hex(Res2));
                goto cleanup;
            }

            if (!BI_copy(Res2, B) || !BI_div(Res1, Res2, A, Res2))
                goto cleanup;
            if (BI_cmp(Res1, Quotient) || BI_cmp(Res2, Remainder)) {
                fprintf(stderr, "ERROR :: Rem == B\n");
                fprintf(stderr, "A :: %s\nB :: %s\n", bi_to_hex(A), bi_to_hex(B));
                fprintf(stderr, "Expected :: %s\nGot :: %s\n", bi_to_hex(Quotient), bi_to_hex(Res1));
                fprintf(stderr, "Expected :: %s\nGot :: %s\n\n", bi_to_hex(Remainder), bi_to_hex(Res2));
                goto cleanup;
            }

            continue;
        }

        ERR("Unexpected line in file");
        fprintf(stderr, "%s\n", buff);
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(B);
    BI_free(Quotient);
    BI_free(Remainder);
    BI_free(Res1);
    BI_free(Res2);
    return ret;
}

int file_test_exp(void)
{
    FILE *fp = fopen("recipies/bi/bi_exp.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_exp.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *E, *Exp, *Res;
    A = BI_new();
    E = BI_new();
    Exp = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("Exp = ", buff, 5) == 0) {
            GET_BI(Exp, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'E') {
            GET_BI(E, buff);

            if (BI_pow(Res, A, E) == 0)
                goto cleanup;
            if (BI_cmp(Res, Exp))
                goto cleanup;

            continue;
        }
        ERR("Unexpected line in file");
        printf("%s\n", buff);
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(E);
    BI_free(Exp);
    BI_free(Res);
    return ret;
}

int file_test_modmul(void)
{
    FILE *fp = fopen("recipies/bi/bi_modmul.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_modmul.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *B, *M, *ModMul , *Res;
    A = BI_new();
    B = BI_new();
    M = BI_new();
    ModMul = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("ModMul = ", buff, 8) == 0) {
            if (!GET_BI(ModMul, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'A') {
            if (!GET_BI(A, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'B') {
            if (!GET_BI(B, buff))
                goto cleanup;
            continue;
        }
        if (buff[0] == 'M') {
            if (!GET_BI(M, buff))
                goto cleanup;

            if (!BI_modmul(Res, A, B, M) || BI_cmp(Res, ModMul))
                goto cleanup;

            if (!BI_copy(Res, A) || !BI_modmul(Res, Res, B, M) || BI_cmp(Res, ModMul))
                goto cleanup;

            if (!BI_copy(Res, B) || !BI_modmul(Res, A, Res, M) || BI_cmp(Res, ModMul))
                goto cleanup;

            continue;
        }
        ERR("Unexpected line in file");
        printf("%s\n", buff);
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(B);
    BI_free(M);
    BI_free(ModMul);
    BI_free(Res);
    return ret;
}

int file_test_modpow(void)
{
    FILE *fp = fopen("recipies/bi/bi_modpow.txt", "r");
    if (!fp) {
        ERR("Could Not Open File \"bi_modpow.txt\"\n");
        return 0;
    }

    char buff[4096];

    BIGINT *A, *E, *M, *ModExp , *Res;
    A = BI_new();
    E = BI_new();
    M = BI_new();
    ModExp = BI_new();
    Res = BI_new();

    int ret = 0;

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (strncmp("ModExp = ", buff, 8) == 0) {
            GET_BI(ModExp, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'E') {
            GET_BI(E, buff);
            continue;
        }
        if (buff[0] == 'M') {
            GET_BI(M, buff);

            if (BI_modpow(Res, A, E, M) == 0)
                goto cleanup;
            if (BI_cmp(Res, ModExp) != 0)
                goto cleanup;

            continue;
        }
        ERR("Unexpected line in file");
        printf("%s\n", buff);
        goto cleanup;
    }
    ret = 1;
cleanup:
    fclose(fp);
    BI_free(A);
    BI_free(E);
    BI_free(M);
    BI_free(ModExp);
    BI_free(Res);
    return ret;
}

int main(void)
{
    if (file_test_sum())
        printf("All bigint addition tests passed\n");
    if (file_test_mul())
        printf("All bigint multiplication tests passed\n");
    if (file_test_sqr())
        printf("All bigint square tests passed\n");
    if (test_lshift1_file())
        printf("All bigint lshift1 tests passed\n");
    if (test_lshift_file())
        printf("All bigint lshift tests passed\n");
    if (test_rshift_file())
        printf("All bigint rshift tests passed\n");
    if (test_div_file())
        printf("All bigint division tests passed\n");
    if (file_test_exp())
        printf("All bigint exponent tests passed\n");
    if (file_test_modmul())
        printf("All bigint modmul tests passed\n");
    if (file_test_modpow())
        printf("All bigint modpow tests passed\n");

    return 0;
}
