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

#include <stdio.h>
#include <stdlib.h>

#include "bi.h"

int hex_to_int(const char c)
{
    if (c >= '0' && c <= '9')
        return c - 48;
    if (c >= 'A' && c <= 'F')
        return c - 55;
    if (c >= 'a' && c <= 'f')
        return c - 87;
    return -1;
}

int hex_to_bi(BIGINT *b, const char *hex)
{
    b->size = 0;
    b->sign = 0;

    int i, num = 0;
    if (hex[0] == '-') {
        b->sign = 1;
        ++num;
        ++hex;
    }
    for (i = 0; hex[i]; i++)
        continue;

    num += i;

    int s = (i - 1) * ((float)4 / word_size) + 1;
    b = bi_expand(b, s);
    
    while (i > 0) {
        WORD w = 0;
        int win = i > word_size / 4 ? word_size / 4 : i;
        while (win > 0) {
            int d = hex_to_int(hex[i - win]);
            w <<= 4;
            w |= d;
            --win;
        }
        b->dig[b->size++] = w;
        i -= word_size / 4;
    }
    BI_correct_top(b);
    return num;
}

char *bi_to_hex(const BIGINT *b)
{
    char *str = malloc(b->size * (word_size / 4) + 2);
    int print = 0, t = 0;

    if (b->size == 0 && b->sign == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (b->sign)
        str[t++] = '-';

    const char *hex_dig = "0123456789abcdef";

    for (int j = b->size - 1; j >= 0; j--) {
        for (int k = word_size - 4; k >= 0; k -= 4) {
            WORD w = (b->dig[j] >> k) & 0xf;
            if (w != 0 || print) {
                str[t++] = hex_dig[w];
                print = 1;
            }
        }
    }
    str[t] = '\0';

    return str;
}

