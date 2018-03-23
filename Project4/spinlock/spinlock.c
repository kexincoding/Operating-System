#include "spinlock.h"

static inline unsigned int xchg(volatile unsigned int *addr, unsigned int newval)
{
    unsigned int result;
    asm volatile("lock; xchgl %0, %1" : "+m" (*addr), "=a" (result) : "1" (newval) : "cc");
    return result;
}

void init(spinlock_t *lock)
{
    lock->flag = 0;
}


void spinlock_acquire(spinlock_t *lock)
{
    while (xchg( (volatile unsigned int *)(&lock->flag), 1 ) == 1);//返回原来的值
}

void spinlock_release(spinlock_t *lock)
{
    lock -> flag = 0;
}
