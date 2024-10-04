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
 * This is a very simple demo which calculates 100!
 */

#include <stdio.h>
#include "bi.h"

int main(void)
{
    BIGINT *n, *fac, *one, *c;
    n = BI_new();
    fac = BI_new();
    one = BI_new();
    c = BI_new();
    if (!n || !fac || !one || !c)
        return 1;
    hex_to_bi(n, "64");
    hex_to_bi(fac, "1");
    hex_to_bi(one, "1");
    hex_to_bi(c, "1");

    while (BI_cmp(c, n) <= 0) {
        BI_mul(fac, fac, c);
        BI_add(c, c, one);
    }

    printf("%s factorial is %s\n", bi_to_hex(n), bi_to_hex(fac));

    BI_free(n);
    BI_free(fac);
    BI_free(one);
    BI_free(c);

    return 0;
}
