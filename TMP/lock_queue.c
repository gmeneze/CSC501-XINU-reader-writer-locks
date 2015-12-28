/* queue.c - dequeue, enqueue */

#include <conf.h>
#include <kernel.h>
#include <lock.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * enqueue  --	insert an item at the tail of a list
 *------------------------------------------------------------------------
 */
int enqueue_lock(process_info_t proc, int lock_num)
{ 
    queue_lock *lptr;
    queue_lock *new_lptr;

    if( lock_arr[lock_num].lock_queue == NULL )    
    {
        lock_arr[lock_num].lock_queue = (queue_lock *)(getmem(sizeof(queue_lock)));   

        lock_arr[lock_num].lock_queue->proc_val = (process_info_t *)(getmem(sizeof(process_info_t))); 
        lock_arr[lock_num].lock_queue->proc_val->type = proc.type;
        lock_arr[lock_num].lock_queue->proc_val->priority = proc.priority;
        lock_arr[lock_num].lock_queue->proc_val->pid = proc.pid;
        lock_arr[lock_num].lock_queue->proc_val->time = proc.time;

        lock_arr[lock_num].lock_queue->next = NULL;

        return OK;
    }
   

    lptr = lock_arr[lock_num].lock_queue;

    while( lptr != NULL )
    {
        if( proc.priority > lptr->proc_val->priority )
        {
            new_lptr = (queue_lock *) getmem(sizeof(queue_lock)); 
            new_lptr->proc_val = (process_info_t *)(getmem(sizeof(process_info_t)));
            new_lptr->proc_val->type =  proc.type;
            new_lptr->proc_val->priority = proc.priority;
            new_lptr->proc_val->pid = proc.pid;
            new_lptr->proc_val->time = proc.time;

            new_lptr->next = lptr;

            new_lptr->prev = lptr->prev;

            if(lptr->prev == NULL)
            {
                 lock_arr[lock_num].lock_queue = new_lptr;
            }

            lptr->prev = new_lptr;     

            return OK;
        } 
 
        if( proc.priority == lptr->proc_val->priority )
        {

            if( (proc.time - lptr->proc_val->time) > 1000 )
            {
                  new_lptr = (queue_lock *) getmem(sizeof(queue_lock)); 
                  new_lptr->proc_val = (process_info_t *)(getmem(sizeof(process_info_t)));
                  new_lptr->proc_val->type =  proc.type;
                  new_lptr->proc_val->priority = proc.priority;
                  new_lptr->proc_val->pid = proc.pid;
                  new_lptr->proc_val->time = proc.time;

                  new_lptr->next = lptr->next;
                  lptr->next = new_lptr;

                  new_lptr->prev = lptr;
                  return OK;
            }else{
 
                  if( proc.type == WRITE )
                  {
                      new_lptr = (queue_lock *) getmem(sizeof(queue_lock)); 
                      new_lptr->proc_val = (process_info_t *)(getmem(sizeof(process_info_t)));
                      new_lptr->proc_val->type =  proc.type;
                      new_lptr->proc_val->priority = proc.priority;
                      new_lptr->proc_val->pid = proc.pid;
                      new_lptr->proc_val->time = proc.time;

                      new_lptr->prev = lptr->prev;

                      if(lptr->prev == NULL)
                      {
                       lock_arr[lock_num].lock_queue = new_lptr;
                      }
                      lptr->prev = new_lptr;
                      new_lptr->next = lptr;                       
                      return OK; 
 
                  }else{
                      new_lptr = (queue_lock *) getmem(sizeof(queue_lock));
                      new_lptr->proc_val = (process_info_t *)(getmem(sizeof(process_info_t)));
                      new_lptr->proc_val->type =  proc.type;
                      new_lptr->proc_val->priority = proc.priority;
                      new_lptr->proc_val->pid = proc.pid;
                      new_lptr->proc_val->time = proc.time;

                      new_lptr->next = lptr->next;
                      lptr->next = new_lptr;

                      new_lptr->prev = lptr;
                      return OK;
                  }
            }
 
        }
        lptr = lptr->next;
    }

    lptr = lock_arr[lock_num].lock_queue; 

    while(lptr->next!=NULL)
    {
       lptr=lptr->next;
    } 

    new_lptr = (queue_lock *) getmem(sizeof(queue_lock));
    new_lptr->proc_val = (process_info_t *)(getmem(sizeof(process_info_t)));
    new_lptr->proc_val->type =  proc.type;
    new_lptr->proc_val->priority = proc.priority;
    new_lptr->proc_val->pid = proc.pid;
    new_lptr->proc_val->time = proc.time;

    new_lptr->next = NULL;
    new_lptr->prev = lptr; 
    lptr->next = new_lptr;

    return OK;

}

/*------------------------------------------------------------------------
 * getfirst_queue_lock  --  remove and return the first process on a list
 *------------------------------------------------------------------------
 */
int getfirst_queue_lock(int lock_num, int *next_status)
{
        int     proc;                   /* first process on the list    */

        queue_lock *lptr = NULL;

        lptr = lock_arr[lock_num].lock_queue;
        proc = lock_arr[lock_num].lock_queue->proc_val->pid; 

        if( lock_arr[lock_num].lock_queue->next != NULL)
        {
               if(((lock_arr[lock_num].lock_queue->proc_val->type) == READ) && ((lock_arr[lock_num].lock_queue->next->proc_val->type) == READ))
               {
                  *next_status = 1;
               }else{
                  *next_status = 0; 
               } 
        }
        else{

              *next_status = 0;
        }
       
        if( lock_arr[lock_num].lock_queue->next != NULL)
        {
           lock_arr[lock_num].lock_queue = lock_arr[lock_num].lock_queue->next;
           lock_arr[lock_num].lock_queue->prev = NULL; 
        }else {
           lock_arr[lock_num].lock_queue = NULL;
        }

/*
           lptr->next = NULL;
           lptr->prev = NULL;
           lptr->proc_val = NULL;
*/
       
        lptr = lock_arr[lock_num].lock_queue;
/*
    while( lptr != NULL )
    {
        kprintf("The pid is: %d, priority: %d, type: %d, time: %d \n", lptr->proc_val->pid, lptr->proc_val->priority, lptr->proc_val->type, lptr->proc_val->time);

     lptr=lptr->next;
    }
*/

        return proc;
}
