/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
int lsignal(int mapping_val)
{
	STATWORD ps;    
        lock_t  *lptr;
        int pid;
        int next_val_status = 1;
	disable(ps);

if (isbadlock(mapping[mapping_val].lock_num) || (lptr= &lock_arr[mapping[mapping_val].lock_num])->lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}

        lptr->lock_val++;

        if(lptr->lock_queue == NULL)
        { 
            restore(ps);
            return(OK);
        }

             while(next_val_status == 1)
             {
                 pid = getfirst_queue_lock(mapping[mapping_val].lock_num, &next_val_status);
                 proctab[pid].pwaitret = OK;

                 if( mapping[mapping_val].type == READ )
                        mapping[mapping_val].count++;

                 ready(pid, RESCHNO);
             }
             resched();
	restore(ps);
	return(OK);
}
