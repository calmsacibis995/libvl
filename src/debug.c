/*
 * libvl - An library for writing init systems.
 * Copyright (C) 2023, 2024 Stefanos Stefanidis, <www.fe32gr23@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "libvl.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

char *
C(char *id)
{
	static char answer[12];
	char *ptr;
	int i;

	for (i=4,ptr = &answer[0]; --i >= 0;id++) {
		if ( isprint(*id) == 0 ) {
			*ptr++ = '^';
			*ptr++ = *id + 0100;
		} else *ptr++ = *id;
	}
	*ptr++ = '\0';
	return(&answer[0]);
}

void
debug(char *format, ...)
{
	va_list va;

	va_start(va, format);
	fprintf(stderr, "DEBUG: ");
	vfprintf(stderr, format, va);
	fprintf(stderr, "\n");
	va_end(va);
}
