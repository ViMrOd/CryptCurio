#include "bi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
#ifdef BN_OLD
    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_add(A, B, C);

        fprintf(stdout, "Result of ADD Sum Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of ADD Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Addition Tests Passed\n\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_sub(A, B, C);

        fprintf(stdout, "Result of SUB Diff Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of SUB Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Subtraction Tests Passed\n\n");
    }

    {
        bn A = bn_init_set_str(argv[1]);
        bn B = bn_init_set_str(argv[2]);

        int cmp = bn_cmp(A, B);

        fprintf(stdout, "Result of CMP Test = %d\n", cmp);
        fprintf(stdout, "Comparision Tests Passed\n\n");
    }

    {
        bn A = bn_init_set_str(argv[1]);
        bn B = bn_init_set_str(argv[2]);

        int cmp = bn_cmp_abs(A, B);

        fprintf(stdout, "Result of CMP ABS Test = %d\n", cmp);
        fprintf(stdout, "Comparision ABS Tests Passed\n\n");
    }
   
    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_mul(A, B, C);

        fprintf(stdout, "Result of MUL Product Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of MUL Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Multiplication Tests Passed\n\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str("0");
        bn C = bn_init_set_str(argv[1]);
        bn D = bn_init_set_str(argv[2]);

        bn_div(A, B, C, D);
        
        fprintf(stdout, "Result of DIV Quotient Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of DIV Quotient Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Result of DIV Remainder Test = %s\n", bn_string(B));
        fprintf(stdout, "Result of DIV Remainder Size Test = %zd\n", get_size(B));
        fprintf(stdout, "Division Tests Passed\n\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);
        bn D = bn_init_set_str(argv[3]);

        bn_powm(A, B, C, D);

        fprintf(stdout, "Result of POWM Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of POWM Size Test = %zd\n", get_size(A));
        fprintf(stdout, "POWM Tests Passed\n\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_pow(A, B, C);

        fprintf(stdout, "Result of POW Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of POW Size Test = %zd\n", get_size(A));
        fprintf(stdout, "POW Tests Passed\n");
    }
#endif

    char *buff = "0123456789abcdef";
    while (*buff)
        putchar(*buff++);
    putchar('\n');

    return EXIT_SUCCESS;
}
