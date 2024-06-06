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
#include <string.h>
#include <utmp.h>
#include <time.h>

int cur_state = -1; /* Current state of "init" */
int prior_state;
int n_prev[NSIG];	/* Number of times previously in state */

char *WTMP = "wtmp";

/*
 * "account" updates entries in /etc/utmp and appends new entries
 * to the end of /etc/wtmp (assuming /etc/wtmp exists).
 * 
 * Argument notes:
 * 
 * program:
 * Name of program in the case of INIT_PROCESSes, otherwise NULL.
 */
void
account(int state, struct PROC_TABLE *process, char *program)
{
	struct utmp utmpbuf;
	struct utmp *u,*oldu;
	extern char *WTMP;
	time_t t;
	FILE *fp;

#ifdef DEBUG
	extern char *C();
	debug("** account ** state: %d id:%s\n",state,C(&process->p_id[0]));
#endif

/* Set up the prototype for the utmp structure we want to write. */
	u = &utmpbuf;
	zero(&u->ut_user[0],sizeof(u->ut_user));
	zero(&u->ut_line[0],sizeof(u->ut_line));

/* Fill in the various fields of the utmp structure. */
	u->ut_id[0] = process->p_id[0];
	u->ut_id[1] = process->p_id[1];
	u->ut_id[2] = process->p_id[2];
	u->ut_id[3] = process->p_id[3];
	u->ut_pid = process->p_pid;

/* Fill the "ut_exit" structure. */
	u->ut_exit.e_termination = (process->p_exit & 0xff);
	u->ut_exit.e_exit = ((process->p_exit >> 8) & 0xff);
	u->ut_type = state;
	t = u->ut_time;
	time(&t);

/* See if there already is such an entry in the "utmp" file. */
	setutent();	/* Start at beginning of utmp file. */
	if ((oldu = getutid(u)) != NULL) {

/* Copy in the old "user" and "line" fields to our new structure. */
		bcopy(&oldu->ut_user[0],&u->ut_user[0],sizeof(u->ut_user));
		bcopy(&oldu->ut_line[0],&u->ut_line[0],sizeof(u->ut_line));
#ifdef	ACCTDEBUG
		debug("New entry in utmp file.\n");
#endif
	}
#ifdef	ACCTDEBUG
	else debug("Replacing old entry in utmp file.\n");
#endif

/* Preform special accounting. Insert the special string into the */
/* ut_line array. For INIT_PROCESSes put in the name of the */
/* program in the "ut_user" field. */
	switch(state) {
	case RUN_LVL :
		u->ut_exit.e_termination = level(cur_state);
		u->ut_exit.e_exit = level(prior_state);
		u->ut_pid = n_prev[cur_state];
		sprintf(&u->ut_line[0],RUNLVL_MSG,level(cur_state));
		break;
	case BOOT_TIME :
		sprintf(&u->ut_line[0],"%.12s",BOOT_MSG);
		break;
	case INIT_PROCESS :
		strncpy(&u->ut_user[0],program,sizeof(u->ut_user));
		break;
	default :
		break;
	}

/* Write out the updated entry to utmp file. */
	if (pututline(u) == (struct utmp *)NULL)
		console("failed write of utmp entry: \"%2.2s\"\n",&u->ut_id[0]);

/* Now attempt to add to the end of the wtmp file.  Do not create */
/* if it doesn't already exist.  **  Note  ** This is the reason */
/* "r+" is used instead of "a+".  "r+" won't create a file, while */
/* "a+" will. */
	if ((fp = fopen(WTMP,"r+")) != NULL) {
		fseek(fp,0L,2);	/* Seek to end of file */
		fwrite((void *)u, sizeof(*u), 1, fp);
		fclose(fp);
	}
}
