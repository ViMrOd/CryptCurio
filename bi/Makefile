CC ?= gcc
FLAGS = -Wall -Wextra -pedantic -std=c89 -O3

OBJECTS = main.o bi_add.o bi_asm.o bi_cmp.o bi_conv.o bi_mul.o bi_shift.o \
bi_stdlib.o 

main : $(OBJECTS)
	CC -o main $(OBJECTS)

main.o : bi.h
bi_add.o : bi.h
bi_asm.o : bi.h
bi_cmp.o : bi.h
bi_conv.o : bi.h
bi_mul.o : bi.h
bi_shift.o : bi.h
bi_stdlib.o : bi.h

tests : 
	@echo "Implement The Tests You Silly Goose!"

clean :
	rm main $(OBJECTS)
