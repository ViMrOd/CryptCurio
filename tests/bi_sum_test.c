#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../bi/bi.h"

int main(int argc, char **argv)
{
    FILE *fp = fopen("recipies/bi/bi_add.txt", "r");
    if (!fp) {
        fprintf(stderr, "Could Not Open File\n");
        return 1;
    }

    char buff[4096], a[4096], b[4096], sum[4096];

    BIGINT *A, *B, *Sum;
    A = BI_new();
    B = BI_new();
    Sum = BI_new();

    while (fgets(buff, 4096, fp)) {
        if (buff[0] == '#' || buff[0] == '\n' || buff[0] == ' ')
            continue;

        if (buff[0] == 'S' && buff[1] == 'u' && buff[2] == 'm') {
            strcpy(sum, &buff[6]);
            sum[strlen(sum) - 1] = '\0';
            continue;
        }
        if (buff[0] == 'A') {
            strcpy(a, &buff[4]);
            a[strlen(a) - 1] = '\0';
            hex_to_bi(A, a);
            assert(strcmp(bi_to_hex(A), a) == 0);
            continue;
        }
        if (buff[0] == 'B') {
            strcpy(b, &buff[4]);
            b[strlen(b) - 1] = '\0';
            hex_to_bi(B, b);
            assert(strcmp(bi_to_hex(B), b) == 0);

            BI_add(Sum, A, B);
            assert(strcmp(bi_to_hex(Sum), sum) == 0);
        }
    }
    
    printf("All BIGINT Addition Tests Passed\n");

    fclose(fp);

    BI_free(A);
    BI_free(B);
    BI_free(Sum);

    return 0;
}
