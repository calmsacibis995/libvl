/*
 * libvl - An library for writing init systems.
 * Copyright (C) 2023 Stefanos Stefanidis, <www.fe32gr23@gmail.com>
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

/*
 * Dummy file to test if libvl works as expected.
 * If it does, it should print the messages listed below, and then exit.
 * If not, there is a bug, and it should be immediately reported.
 * 
 * 1. "INIT: Hello!"
 * 2. "INIT: 4" (from variable with %d)
 * 3. "INIT: Hello!" (from variable with %s)
 * 4. "DEBUG: This is a test!" 
 * 5. "DEBUG: This is a test!" (from variable with %s)
 */

#include <libvl.h>

int
main()
{
	char *string = "Hello!";
	char *dbgstr = "This is a test!";
	int val = 4;
	
	console("Hello!\n");
	console("%d\n", val);
	console("%s\n", string);
	debug("This is a test!\n");
	debug("%s\n", dbgstr);

	return(0);	
}
