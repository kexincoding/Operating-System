#include "ticket.h"

void lock_init(lock_t *lock)
{
    lock->ticket = 0;
    lock->turn= 0;
}

void alock(lock_t *lock)
{
    int myturn =  __sync_fetch_and_add(&lock->ticket,1);
    while (lock->turn != myturn)
        ; 
}

void unlock(lock_t *lock)
{
    lock->turn = lock->turn + 1;
}


