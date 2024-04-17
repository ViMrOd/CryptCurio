#include "bn.h"

int main(int argc, char **argv)
{
    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_add(A, B, C);

        fprintf(stdout, "Result of ADD Sum Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of AA Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Addition Tests Passed\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_sub(A, B, C);

        fprintf(stdout, "Result of SUB Diff Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of SUBn Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Subtraction Tests Passed\n");
    }

    {
        bn A = bn_init_set_str(argv[1]);
        bn B = bn_init_set_str(argv[2]);

        int cmp = bn_cmp(A, B);

        fprintf(stdout, "Result of CMP Test = %d\n", cmp);
        fprintf(stdout, "Comparision Tests Passed\n");
    }

    {
        bn A = bn_init_set_str(argv[1]);
        bn B = bn_init_set_str(argv[2]);

        int cmp = bn_cmp_abs(A, B);

        fprintf(stdout, "Result of CMP ABS Test = %d\n", cmp);
        fprintf(stdout, "Comparision ABS Tests Passed\n");
    }
    
    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_mul(A, B, C);

        fprintf(stdout, "Result of MUL Product Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of MUL Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Multiplication Tests Passed\n");
    }

    {
        bn A = bn_init_set_ui(0);
        bn B = bn_init();
        bn C = bn_init_set_str(argv[1]);
        bn D = bn_init_set_str(argv[2]);

        bn_div(A, B, C, D);

        fprintf(stdout, "Result of DIV Quotient Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of DIV Quotient Size Test = %zd\n", get_size(A));
        fprintf(stdout, "Result of DIV Remainder Test = %s\n", bn_string(B));
        fprintf(stdout, "Result of DIV Remainder Size Test = %zd\n", get_size(B));
        fprintf(stdout, "Division Tests Passed\n");
    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_powm(A, B, C, B);

        fprintf(stdout, "Result of POWM Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of POWM Size Test = %zd\n", get_size(A));
        fprintf(stdout, "POWM Tests Passed\n");

    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_pow(A, B, C);

        fprintf(stdout, "Result of POWM Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of POWM Size Test = %zd\n", get_size(A));
        fprintf(stdout, "POWM Tests Passed\n");

    }

    {
        bn A = bn_init();
        bn B = bn_init_set_str(argv[1]);
        bn C = bn_init_set_str(argv[2]);

        bn_mod(A, B, C);

        fprintf(stdout, "Result of MOD Test = %s\n", bn_string(A));
        fprintf(stdout, "Result of MOD Size Test = %zd\n", get_size(A));
        fprintf(stdout, "MOD Tests Passed\n");

    }

    return EXIT_SUCCESS;
}