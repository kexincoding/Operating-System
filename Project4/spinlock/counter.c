#include "counter.h"

void counter_init(counter_t * c, int value)
{
    c->value = value;
    init(&c->lock);
}
void counter_increment(counter_t *c)
{
    spinlock_acquire(&c->lock);
    c->value++;
    spinlock_release(&c->lock);
}

void counter_decrement(counter_t *c)
{
    spinlock_acquire(&c->lock);
    c->value--;
    spinlock_release(&c->lock);
}

int counter_get_value(counter_t *c)
{
    spinlock_acquire(&c->lock);
    int rc = c->value;
    spinlock_release(&c->lock);
    return rc;
}
