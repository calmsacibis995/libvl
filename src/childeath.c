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
#include <sys/wait.h>

void
childeath(void)
{
	union WAKEUP wakeup;
	struct PROC_TABLE *process;
	int pid;
	int status;

/* Perform wait to get the process id of the child who died and */
/* then scan the process table to see if we are interested in */
/* this process. **Note** if a super-user sends the SIGCLD signal */
/* to "init", the following wait will not immediately return */
/* and "init" will be inoperative until one of its child really */
/* does die. */
	pid = wait(&status);

#ifdef	DEBUG
	debug("childeath: pid %d, status 0x%x\n",pid,status);
	debug("childeath: pid %d, status 0x%x\n",pid,status);
#endif

	for (process= &proc_table[0]; process < &proc_table[NPROC];process++) {
		if ((process->p_flags & (OCCUPIED)) == OCCUPIED && process->p_pid == pid) {

/* Mark this process as having died and store the exit status. */
/* Also set the wakeup flag for a dead child and break out of */
/* loop. */
			process->p_flags &= ~LIVING;
			process->p_exit = status;
			wakeup.w_flags.w_childdeath = 1;
			break;
		}
	}

#ifdef	DEBUG
	if (process == &proc_table[NPROC])
		debug("Didn't find process %d.\n", pid);
#endif

/* Reset the child death signal routine. */
	signal(SIGCLD,childeath);
}
