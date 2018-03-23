#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

typedef struct
{
    int mutex;
} mutex_t;


static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3);
void mutex_init(mutex_t *lock);
void mutex_acquire (mutex_t *lock);
void mutex_release (mutex_t *lock);
