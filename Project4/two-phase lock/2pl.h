#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>




typedef struct
{
    int mutex;
} lock_t;

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3);
void mutex_init(lock_t *lock);
void mutex_lock (lock_t *lock);
void mutex_unlock (lock_t *lock);
void * mythread(void *arg);
