/* screate.c - lcreate, newlock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newlock();

/*------------------------------------------------------------------------
 * lcreate  --  create and initialize a lock, returning its id
 *------------------------------------------------------------------------
 */
int lcreate()
{
	STATWORD ps;    
	int	lock_val;

	disable(ps);
        lock_val=newlock(); 
	if (lock_val == SYSERR ) {
		restore(ps);
		return(SYSERR);
	}

        mapping[++mapping_num].lock_num = lock_val;

	/* lqhead and lqtail were initialized at system startup */
	restore(ps);
	return(mapping_num);
}

/*------------------------------------------------------------------------
 * newlock  --  allocate an unused lock and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newlock()
{
	int	lock_num;
	int	i;

	for (i=0 ; i<NLOCK ; i++) {
                if ( (lock_num=nextlock--) <= 0) 
			nextlock = NLOCK-1;
		if (lock_arr[lock_num].lstate==LFREE) {
			lock_arr[lock_num].lstate = LUSED;
			return(lock_num);
		}
	}
	return(SYSERR);
}
