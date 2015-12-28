/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 * lock  --  make current process wait on a lock
 *------------------------------------------------------------------------
 */
int lock (int ldes1, int type, int priority)
{
	STATWORD ps;    
	disable(ps);
        int status;
        plock_t *plock_q;
        plock_t *new_plock_q;

        if(mapping[ldes1].status == DELETED)
        {
             return SYSERR;
        } 

        status = lwait(ldes1, priority, type); 

        plock_q = proctab[currpid].proc_lock_q;

        if( plock_q->mapping_num == -1 )
        {
                plock_q->mapping_num = ldes1;
                restore(ps);
                return status;
        }

        while(plock_q->next != NULL)
        {
               plock_q = plock_q->next;
        }

        new_plock_q = (plock_t *) getmem(sizeof(plock_t));
        new_plock_q->mapping_num = ldes1;
        new_plock_q->next = NULL;
        new_plock_q->prev = plock_q;

        plock_q->next = new_plock_q;

        if(status != OK) 
        {
           printf("lock failed, the mapping is bad or the lock is free");
           restore(ps);
           return status;
        }

	restore(ps);
	return(OK);
}
