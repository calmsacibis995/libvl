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

#ifndef _LIBVL_H
#define _LIBVL_H

#include <signal.h>

#define DEAD_PROCESS	8

#define	TRUE	1
#define	FALSE	0
#define	FAILURE	-1

#define	NPROC	100

#define	OCCUPIED	01
#define	LIVING		02
#define	NOCLEANUP	04
#define	NAMED		010
#define	DEMANDREQUEST	020
#define	TOUCHED		040
#define	WARNED		0100
#define	KILLED		0200

#define	NULLPROC	((struct PROC_TABLE *)(0))
#define	NO_ROOM		((struct PROC_TABLE *)(FAILURE))

#define	FULLTABLE	0

#define	SPECIALPID	1

#define	MAX_E_TYPES	2

#define BOOT_MSG "system boot"
#define RUNLVL_MSG "run-level %c"

/*
 * Correspondence of signals to init actions.
 */

#define LVLQ	SIGHUP
#define	LVL0	SIGINT
#define	LVL1	SIGQUIT
#define	LVL2	SIGILL
#define	LVL3	SIGTRAP
#define	LVL4	SIGIOT
#define	LVL5	SIGTERM
#define	LVL6	SIGFPE
#define	SINGLE_USER	SIGBUS
#define	LVLa	SIGSEGV
#define	LVLb	SIGSYS
#define	LVLc	SIGPIPE

struct PROC_TABLE {
	char p_id[4];	/* Four letter unique id of process */
	unsigned short p_pid;	/* Process id */
	short p_count;	/* How many respawns of this command in the current series. */
	long p_time;	/* Start time for a series of respawns */
	short p_flags;	/* Process flags */
	short p_exit;	/* Exit status of a process which died */
};

extern struct PROC_TABLE proc_table[NPROC];	/* Table of active processes */

static struct ERRORTIMES {
	long e_time;	/* Time of last message. */
	long e_max;		/* Amount of time to wait until next message. */
} err_times[MAX_E_TYPES] = {
	0L,120L,0L,120L
};

union WAKEUP {
	struct WAKEFLAGS {
		unsigned w_usersignal : 1;	/* User sent signal to "init" */
		unsigned w_childdeath : 1;	/* An "init" child died */
		unsigned w_powerhit : 1;	/* The OS experienced powerfail */
	}	w_flags;
	int w_mask;
};

extern union WAKEUP wakeup;

extern int cur_state;
extern int own_pid;	/* This is the value of our own pid. */

extern char *DBG_FILE, *WTMP;

/*
 * Function declarations.
 */
extern void debug(char *format, ...);
extern void console(char *format, ...);
extern void account(int state, struct PROC_TABLE *process, char *program);
extern int error_time(int type);
extern struct PROC_TABLE *efork(struct PROC_TABLE *process, int modes);
extern void timer(int waitime);
extern void childeath();
extern void zero(char *adr, int size);
extern char level(int state);
extern long waitproc(struct PROC_TABLE *process);

#endif /* _LIBVL_H */
