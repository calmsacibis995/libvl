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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

int own_pid;

static const char *console_name = "/dev/console";

static void
open_console(void)
{
	int fd;

	if ((fd = open(console_name, O_RDWR)) < 0)
		fd = open("/dev/null", O_RDWR);

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
}

/*
 * "console" forks a child if it finds that it is the main "init"
 * and outputs the requested message to the system console.
 * Note that the number of arguments passed to "console" is
 * determined by the print format.
 */
void
console(char *format, ...)
{
	struct PROC_TABLE *process;
	char outbuf[BUFSIZ];
	va_list va;

	/*
	 * Are with the original "init"?  If so, fork
	 * a child to do the printing for us.
	 */
	if (own_pid == SPECIALPID) {
		signal(SIGCLD,SIG_DFL);
		while ((process = efork(NULLPROC,NOCLEANUP)) == NO_ROOM)
			timer(5);
		signal(SIGCLD, (void(*)(int))childeath);
		if (process == NULLPROC) {
			/*
			 * Close the standard descriptors and open the system console.
			 */
			open_console();
			setbuf(stdout,&outbuf[0]);

			/*
			 * Output the message to the console.
			 */
			va_start(va, format);
			fprintf(stdout,"\nINIT: ");
			vfprintf(stdout, format, va);
			fflush(stdout);
			va_end(va);
			exit(0);

		/*
		 * The parent waits for the message to complete.
		 */
		} else
			while(waitproc(process) == FAILURE);
	/*
	 * If we are some other "init", then just print
	 * directly to the standard output.
	 */
	} else {
		setbuf(stdout,&outbuf[0]);
		va_start(va, format);
		fprintf(stdout, "\nINIT: ");
		vfprintf(stdout, format, va);
		fflush(stdout);
		va_end(va);
	}
}
