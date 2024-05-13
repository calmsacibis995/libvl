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
#include <unistd.h>

/*
 * "waitproc" waits for a specified process to die.  For this
 * routine to work, the specified process must already in
 * the proc_table.  "waitproc" returns the exit status of the
 * specified process when it dies.
 */
long
waitproc(struct PROC_TABLE *process)
{
	int answer;

/* Wait around until the process dies. */
	if (process->p_flags & LIVING)
		pause();
	if (process->p_flags & LIVING)
		return(FAILURE);

/* Make sure to only return 16 bits so that answer will always */
/* be positive whenever the process of interest really died. */
	answer = (process->p_exit & 0xffff);

/* Free the slot in the proc_table. */
	process->p_flags = 0;
	return(answer);
}
