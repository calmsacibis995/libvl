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

#include "libvl.h"
#include <stddef.h>
#include <unistd.h>
#include <errno.h>

struct PROC_TABLE proc_table[NPROC];

/*
 * "efork" forks a child and the parent inserts the process in
 * its table of processes that are directly a result of forks
 * that it has performed.  The child just changes the "global"
 * with the process id for this process to it's new value.
 * 
 * If "efork" is called with a pointer into the proc_table
 * it uses that slot, otherwise it searches for a free slot.
 * Whichever way it is called, it returns the pointer to the
 * proc_table entry.
 */
struct PROC_TABLE *
efork(register struct PROC_TABLE *process, int modes)
{
	register int childpid;
	register struct PROC_TABLE *proc;
	int i;
	extern void childeath(int);
	void (*oldroutine)(int);

/* Freshen up the proc_table, removing any entries for dead */
/* processes that don't have the NOCLEANUP set.  Perform the */
/* necessary accounting. */
	for (proc= &proc_table[0]; proc < &proc_table[NPROC]; proc++) {
		if ((proc->p_flags & (OCCUPIED | LIVING | NOCLEANUP)) == (OCCUPIED)) {
#ifdef	DEBUG
			debug("efork- id:%s pid: %d time: %lo %d %o %o\n",
				C(&proc->p_id[0]),proc->p_pid, proc->p_time,
				proc->p_count, proc->p_flags,proc->p_exit);
#endif

/* Is this a named process?  If so, do the necessary bookkeeping. */
			if (proc->p_flags & NAMED) account(DEAD_PROCESS,proc,NULL);

/* Free this entry for new usage. */
			proc->p_flags = 0;
		}
	}

	while((childpid = fork()) == FAILURE) {
/* Shorten the alarm timer in case someone else's child dies and */
/* free up a slot in the process table. */
		alarm(5);

/* Wait for some children to die.  Since efork() is normally */
/* called with SIGCLD in the default state, reset it to catch */
/* so that child death signals can come in. */

		oldroutine = signal(SIGCLD,childeath);
		pause();
		signal(SIGCLD,oldroutine);
	}
	if (childpid != 0) {

/* If a pointer into the process table was not specified, then */
/* search for one. */
		if (process == NULLPROC) {
			for (process= &proc_table[0]; process->p_flags != 0 && process < &proc_table[NPROC];process++ );
			if (process == &proc_table[NPROC]) {
				if (error_time(FULLTABLE))
					console("Internal process table is full.\n");
				return(NO_ROOM);
			}
			process->p_time = 0L;
			process->p_count = 0;
		}
		process->p_id[0] = '\0';
		process->p_id[1] = '\0';
		process->p_id[2] = '\0';
		process->p_id[3] = '\0';
		process->p_pid = childpid;
		process->p_flags = (LIVING | OCCUPIED | modes);
		process->p_exit = 0;
	} else {
		/* Reset child's concept of its own process id. */
		own_pid = getpid();
		process = NULLPROC;

		/* Reset all signals to the system defaults. */
		for (i=SIGHUP; i <= SIGPWR;i++) signal(i,SIG_DFL);
	}
	return(process);
}
