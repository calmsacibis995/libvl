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
#include <time.h>
#include <unistd.h>

/*
 * Flag set to TRUE by alarm interupt routine
 * each time an alarm interupt takes place.
 */
int time_up;

void
setimer(int timelimit)
{
	alarm(timelimit);
	time_up = FALSE;
}

/*
 * "timer" is a substitute for "sleep" which uses "alarm" and "pause".
 */
void
timer(int waitime)
{
	extern union WAKEUP wakeup;

	setimer(waitime);
	while (time_up == FALSE) pause();
}

/*
 * "error_time" keeps a table of times, one for each time of
 * error that it handles.  If the current entry is 0 or the
 * elapsed time since the last error message is large enough,
 * "error_time" returns TRUE, otherwise it returns FALSE.
 */
int
error_time(int type)
{
	long curtime;
	extern struct ERRORTIMES err_times[];

	time(&curtime);
	if (err_times[type].e_time == 0
	   || (curtime - err_times[type].e_time >= err_times[type].e_max)) {
		err_times[type].e_time = curtime;
		return(TRUE);
	} else
		return(FALSE);
}
