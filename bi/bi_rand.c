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
#include <unistd.h>
#include <fcntl.h>

#include "bi.h"

int get_rand_bytes(unsigned char *buff, int num_bytes)
{
    int f, bytes_read;

    f = open("/dev/urandom", O_RDONLY);
    if (f == -1) {
        perror("Failed to open /dev/urandom");
        return 0;
    }

    if (buff == NULL) {
        close(f);
        return 0;
    }

    bytes_read = read(f, buff, num_bytes);
    if (bytes_read != num_bytes) {
        perror("Failed to read from /dev/urandom");
        close(f);
        return 0;
    }

    close(f);
    return 1;
}

int BI_rand(BIGINT *r, int bits, int exact, int bottom)
{   
    int bytes = (bits + 7) / 8;
    unsigned char *buff = malloc(bytes);

    if (!get_rand_bytes(buff, bytes))
        return 0;

    unsigned char bit = (bits - 1) % 8;
    unsigned char mask = ~(0xff << (bit + 1));

    if (exact)
        buff[0] |= (1 << bit);

    if (bottom)
        buff[bytes - 1] |= 1;

    buff[0] &= mask;

    r = bi_expand(r, (bits + word_size - 1) / word_size);
    if (!r)
        return 0;
    r->size = 0;

    for (int i = 0; i < bytes; i++) {
        if (!BI_lshift(r, r, 8))
            return 0;
        r->dig[0] |= buff[i];
        if (r->size == 0)
            ++r->size;
    }

    BI_correct_top(r);
    free(buff);

    return 1;
}

int BI_rand_range(BIGINT *r, const BIGINT *rng, int exact, int bottom)
{   
    int n = BI_num_bits(rng);

    do {
        if (!BI_rand(r, n, exact, bottom))
            return 0;
    } while (BI_cmp(r, rng) >= 0);

    return 1;
}
