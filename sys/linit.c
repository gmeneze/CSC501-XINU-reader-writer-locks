/* init.c - init_dev */

#include <lock.h>
#include <conf.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 *  init_dev  -  initialize a device
 *------------------------------------------------------------------------
 */
void linit()
{
int i;
lock_t *lptr = NULL;
        for (i=0 ; i<NLOCK ; i++) {      /* initialize locks */
                (lptr = &lock_arr[i])->lstate = LFREE;
                lptr->lock_val = 1;

                lock_arr[i].lock_queue = NULL; 
                lptr->lock_queue->proc_val = NULL; 
                lptr->lock_queue->next = NULL;
                lptr->lock_queue->prev = NULL;
             } 

        for (i=0 ; i<NMAPPING ; i++) {      /* initialize locks */
                mapping[i].lock_num = -1; 
                mapping[i].type = -1;
                mapping[i].count = 0;
                mapping[i].status = 1;
        }       
}
