/* lock.h */
#ifndef _LOCK_H_
#define _LOCK_H_

#define	NLOCK		50	/* number of locks, if not defined	*/
#define NLOCKS          50
#define NMAPPING        200     /* number of mappings */

#define	LFREE	'\01'		/* this semaphore is free		*/
#define	LUSED	'\02'		/* this semaphore is used		*/
#define READ    1               /* The type of lock is read             */
#define WRITE   2               /* The type of lock is write            */

typedef struct _mapping_t{
        int lock_num;   
        int type;  // type of lock, currently held.
        int count; // number of locks held, only when this is 0 can the lock be acquired by writer.
        int status; 
} mapping_t;

typedef struct _process_info_t{
        int pid; 
        int priority;
        int type;   
        int time;  
}process_info_t;

typedef struct  _queue_lock    {               /* one for each process plus two for    */
                                      /* each list                            */
        process_info_t *proc_val;           /* key on which the queue is ordered    */
        struct _queue_lock     *next;          /* pointer to next process or tail      */
        struct _queue_lock     *prev;          /* pointer to previous process or head  */
}queue_lock;

typedef struct	_lock_t{	        	/* lock table entry		        */
	char    lstate;		        /* the state LFREE or LUSED		*/
        int     lock_val;                /* Semaphore index corresponding to lock */
        queue_lock    *lock_queue;
}lock_t;

#define isbadlock(s)     (s<0 || s>=NLOCK)

extern	lock_t	lock_arr[];
extern	int	nextlock;

extern mapping_t mapping[];
extern int mapping_num;

/* Function declarations */
void linit();
int lcreate();
int enqueue_lock(process_info_t proc, int lock_num);
int getfirst_queue_lock(int lock_num, int *next_status);
int lsignal(int lock_num);
int lwait(int lock_num, int priority, int type);
int ldelete(int lock_val);
int releaseall(int , long, ...);
int lock (int ldes1, int type, int priority);

#endif
