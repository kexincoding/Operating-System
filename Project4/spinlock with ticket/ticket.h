#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>

typedef struct __lock_t
{
    int ticket;
    int turn;
} lock_t;


void lock_init(lock_t *lock);
void alock(lock_t *lock);
void unlock(lock_t *lock);
