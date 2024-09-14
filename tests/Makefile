# CryptCurio 
# Copyright (C) 2024 ViMrOd

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC ?= gcc
FLAGS = -Wall -Wextra -pedantic -std=c89 -O3

OBJECTS = bi_sum_test.o bi_add.o bi_asm.o bi_cmp.o bi_conv.o bi_stdlib.o bi_mul.o 

run : tests
	./tests

tests : $(OBJECTS)
	CC $(FLAGS) -o tests $(OBJECTS)

bi_sum_test.o :

bi_add.o : ../bi/bi.h ../bi/bi_add.c
	CC -c ../bi/bi_add.c
bi_asm.o : ../bi/bi.h ../bi/bi_asm.c
	CC -c ../bi/bi_asm.c
bi_conv.o : ../bi/bi.h ../bi/bi_conv.c
	CC -c ../bi/bi_conv.c
bi_cmp.o : ../bi/bi.h ../bi/bi_cmp.c
	CC -c ../bi/bi_cmp.c
bi_stdlib.o : ../bi/bi.h ../bi/bi_stdlib.c
	CC -c ../bi/bi_stdlib.c
bi_mul.o : ../bi/bi.h ../bi/bi_mul.c
	CC -c ../bi/bi_mul.c

clean :
	rm tests $(OBJECTS)
