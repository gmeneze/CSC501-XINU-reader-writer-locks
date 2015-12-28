/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 * lwait  --  make current process wait on a lock
 *------------------------------------------------------------------------
 */
SYSCALL	lwait(int mapping_val, int priority, int type)
{
	STATWORD ps;    
	lock_t	*lptr;
        process_info_t lqueue;
        struct	pentry	*pptr;
        
	disable(ps);
if (isbadlock(mapping[mapping_val].lock_num) || (lptr= &lock_arr[mapping[mapping_val].lock_num])->lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}

        if((mapping[mapping_val].type == READ) && (type == READ))
        {
            int max_writer_priority=0;
            queue_lock *lptr=NULL;
            lptr = lock_arr[mapping[mapping_val].lock_num].lock_queue;
          
            while(lptr != NULL)
            {
                 if((lptr->proc_val->priority > max_writer_priority) && (lptr->proc_val->type == WRITE)) 
                 {
                    max_writer_priority = lptr->proc_val->priority;
                 }
                 lptr=lptr->next;
            }  
 
            if( priority >= max_writer_priority)  
            {
                 mapping[mapping_val].count++;

                restore(ps);
                return(OK);             
            }
        }

        if(lptr->lock_val > 0)
        { 
             lptr->lock_val = lptr->lock_val-1; 	
             mapping[mapping_val].type = type;
 
             if(mapping[mapping_val].type == READ) 
                mapping[mapping_val].count=1; 

            restore(ps);
            return(OK);
        }
        else {
             lptr->lock_val--; 
   	     (pptr = &proctab[currpid])->pstate = PRWAIT;
              lqueue.pid = currpid;
              lqueue.priority = priority;
              lqueue.type = type;  
              lqueue.time = ctr1000;
	      enqueue_lock(lqueue,mapping[mapping_val].lock_num);

	     resched();
	     restore(ps); 
	     return proctab[currpid].pwaitret; 
	}
	restore(ps);
	return(OK);
}
