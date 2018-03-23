#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include "2pl.h"


typedef struct __counter_t
{
    int value;
    lock_t lock;
} counter_t;

void counter_init(counter_t * c, int value);
void counter_increment(counter_t *c);
void counter_decrement(counter_t *c);
int counter_get_value(counter_t *c);
