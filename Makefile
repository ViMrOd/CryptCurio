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
# along with this program.  If not, see <https://www.gnu.org/licenses/>

.PHONY : all build tests clean

all : build

build :
	$(MAKE) -C bi

tests :
	$(MAKE) -C tests
	cd tests && ./bi_test

clean :
	$(MAKE) -C bi clean
	$(MAKE) -C tests clean



