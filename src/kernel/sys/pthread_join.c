/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/config.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/mm.h>
#include <nanvix/pm.h>
#include <sys/types.h>
#include <errno.h>

/* Sleeping chain. */
// PRIVATE struct process *chain = NULL;

/*
 * @brief Join the specified thread.
 */
PUBLIC int sys_pthread_join(pthread_t thread, void **retval)
{
	struct thread *t;

repeat:
	kprintf("sys_pthread_join pthread_t %d, repeat", thread);

	/* Look for thread to join. */
	for (t = FIRST_THRD; t <= LAST_THRD; t++)
	{
		/* Found. */
		if (t->tid == (tid_t)thread)
		{
			if (t->state == THRD_DEAD || t->state == THRD_STOPPED)
			{
				kprintf("thread %d found dead", t->tid);
				*retval = t->retval;
				return(0);
			}
			else
			{
				kprintf("thread %d found still running", t->tid);
			}
		}
	}

	/* Wait for a future thread to exit. */
	curr_thread->state = THRD_WAITING;
	yield();

	/* Repeat to check if the exiting thread is the one. */
	goto repeat;

	return (-EINTR);
}