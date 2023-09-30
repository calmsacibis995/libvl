#
# libvl - An library for writing init systems.
# Copyright (C) 2023 Stefanos Stefanidis, <www.fe32gr23@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

all: test

LDPATH = ../src
LIB = vl
CFLAGS = -O -I../src -L$(LDPATH) -l$(LIB)

test:
	gcc $(CFLAGS) -o test test.c

clean:
	rm test

.PHONY: all test
