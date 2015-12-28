/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * ldelete  --  delete a lock by releasing its table entry
 *------------------------------------------------------------------------
 */
int ldelete(int mapping_val)
{
	STATWORD ps;    
        int lock_val;
	lock_t	*lptr;
        queue_lock *lqueue;

	disable(ps);

        lock_val = mapping[mapping_val].lock_num;
        mapping[mapping_val].count = 0;
        mapping[mapping_val].status = DELETED;

	if (lock_arr[lock_val].lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &lock_arr[lock_val];
        lqueue = lptr->lock_queue;
        
	lptr->lstate = LFREE;

        while(lqueue != NULL)
        {
          proctab[lqueue->proc_val->pid].pwaitret = DELETED;
          ready(lqueue->proc_val->pid,RESCHNO);
          lqueue = lqueue->next;
        }
        lock_arr[lock_val].lock_queue = NULL;
        resched();

	restore(ps);
	return(OK);
}
