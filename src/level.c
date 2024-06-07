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

char
level(int state)
{
	char answer;

	switch(state) {
	case LVL0 :
		answer = '0';
		break;
	case LVL1 :
		answer = '1';
		break;
	case LVL2 :
		answer = '2';
		break;
	case LVL3 :
		answer = '3';
		break;
	case LVL4 :
		answer = '4';
		break;
	case LVL5 :
		answer = '5';
		break;
	case LVL6 :
		answer = '6';
		break;
	case SINGLE_USER :
		answer = 'S';
		break;
	case LVLa :
		answer = 'a';
		break;
	case LVLb :
		answer = 'b';
		break;
	case LVLc :
		answer = 'c';
		break;
	default :
		answer = '?';
		break;
	}
	return(answer);
}
