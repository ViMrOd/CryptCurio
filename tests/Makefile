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
FLAGS = -Wall -Wextra -Wpedantic -O3
INCLUDE = -I../bi
LIB_PATH = -L../bi
LIB = -lbi

.PHONY :  all help clean

all : bi_test

help :
	@echo "Availiable Options"
	@echo "  bi_test - Build BIGINT tests"
	@echo "  clean   - Clean BIGINT test object files"

bi_test : bi_test.c
	CC $(FLAGS) $< $(LIB_PATH) $(LIB) $(INCLUDE) -o $@

clean :
	rm bi_test
