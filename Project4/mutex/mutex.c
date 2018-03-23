#include "mutex.h"

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
    return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}


void mutex_init(mutex_t *lock)
{
    lock->mutex = 0;
}

void mutex_acquire (mutex_t *lock)
{
    int v;
    if (__sync_fetch_and_or(&lock->mutex, 0x80000000) >= 0)
        return;
    __sync_fetch_and_add(&lock->mutex,1);
    while (1)
    {
        if (__sync_fetch_and_or(&lock->mutex, 0x80000000) >= 0)
        {
            __sync_fetch_and_sub(&lock->mutex,1);
            return;
        }
        v = lock->mutex;
        if (v >= 0)
            continue;
        sys_futex(&(lock->mutex),FUTEX_WAIT, v,0,0,0);

    }

}



void mutex_release (mutex_t *lock)
{
    if (__sync_add_and_fetch(&lock->mutex, 0x80000000) == 0)
        return;
    sys_futex(&(lock->mutex),FUTEX_WAKE, 1,0,0,0);
}

mutex_t lock;


