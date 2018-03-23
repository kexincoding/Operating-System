#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "ticket.h"



typedef struct __node_t
{
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t
{
    node_t *head;
    lock_t lock;
} list_t;


void list_init(list_t *list);
void list_insert(list_t *list, unsigned int key);
void list_delete(list_t *list, unsigned int key);
void *list_lookup(list_t *list, unsigned int key);
