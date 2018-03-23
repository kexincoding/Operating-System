#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

typedef struct __lock_t
{
    int flag;
} spinlock_t;



void init(spinlock_t *lock);
void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);
