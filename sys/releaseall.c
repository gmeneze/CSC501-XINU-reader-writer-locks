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
int releaseall (numlocks, args)
        int     numlocks;              /* procedure address            */
        long    args;                   /* arguments (treated like an   */
                                        /* array in the code)           */
{
	STATWORD ps;    
	plock_t	*proc_lock;
	disable(ps);
        int status;
        int i=0;
        int found=0;
        unsigned long *a;

        proc_lock = proctab[currpid].proc_lock_q;

        for(i = numlocks-1; i >= 0; i--)
        {
           a = (unsigned long *)(&args) + i;

           proc_lock = proctab[currpid].proc_lock_q;        
           found=0;

           while( proc_lock != NULL)
           {
              if( proc_lock->mapping_num == *a )
              {

/*                  kprintf("################The count is: %d, lock: %d#######\n", mapping[proc_lock->mapping_num].count, mapping[proc_lock->mapping_num].lock_num); */
                  if( mapping[proc_lock->mapping_num].count > 0)
                  {
                       mapping[proc_lock->mapping_num].count--; 
                  }
       
                  if( mapping[proc_lock->mapping_num].count == 0 )
                  {
                       status = lsignal(*a);       
                  }

                  if( proc_lock->prev == NULL )
                  {
                      proctab[currpid].proc_lock_q = proc_lock->next;
                      proc_lock->next = NULL;
                  }
                  else
                  {
                      proc_lock->prev->next = proc_lock->next;
                      proc_lock->prev =NULL;
                      proc_lock->next =NULL; 
                  }
                  found=1;  
                  break;
              } 
              proc_lock = proc_lock->next;
           } 
           if( found == 0 ) 
             status = SYSERR; 
        }
   
        if(status == SYSERR) 
        {
           restore(ps);
           return SYSERR;
        }

	restore(ps);
	return(OK);
}
