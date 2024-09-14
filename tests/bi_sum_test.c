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

#include "../bi/bi.h"

int get_bi(BIGINT *b, const char *op, const char *line)
{
    int i = strlen(op) + 3; /* + 3 comes from 3 chars in " = " */
    char l[4096];
    strcpy(l, &line[i]);
    l[strlen(l) - 1] = '\0'; /* Strip newline char */
    hex_to_bi(b, l);
    assert(strcmp(bi_to_hex(b), l) == 0);
    return 1;
}

#define GET_BI(__a, __b) get_bi(__a, #__a, __b)

#define ERR(__msg) fprintf(stderr, "[ERROR] %s\n", __msg)

int file_test_sum(void)
{
    FILE *fp = fopen("recipies/bi/bi_add.txt", "r");
    if (!fp) {
        ERR("Could not open file \"bi_add.txt\"\n");
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
        ERR("Unexpected line in test file\nCleaning up\n");
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
            GET_BI(Product, buff);
            continue;
        }
        if (buff[0] == 'A') {
            GET_BI(A, buff);
            continue;
        }
        if (buff[0] == 'B') {
            GET_BI(B, buff);

            BI_mul(Res, A, B);
            if (BI_cmp(Res, Product) != 0)
                goto cleanup;
            continue;
        }
        ERR("Unexpected symbol in file\nCleaning up\n");
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

int main(void)
{
    if (file_test_sum())
        printf("All bigint addition tests passed\n");
    if (file_test_mul())
        printf("All bigint multiplication tests passed\n");

    return 0;
}
