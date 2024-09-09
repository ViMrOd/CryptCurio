CC ?= gcc
FLAGS = -Wall -Wextra -pedantic -std=c89 -O3

OBJECTS = bi_sum_test.o bi_add.o bi_asm.o bi_cmp.o bi_conv.o bi_stdlib.o

run : tests
	./test

tests : ../bi/bi.h $(OBJECTS)
	CC -o test $(OBJECTS)

bi_sum_test.o : ../bi/bi.h

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

clean :
	rm test $(OBJECTS)
