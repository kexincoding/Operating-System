#include "counter.h"

void counter_init(counter_t * c, int value)
{
    c->value = value;
	lock_init(&c->lock);
	//c->threshold = threshold;
}
void counter_increment(counter_t *c)
{
	alock(&c->lock);
	c->value++;
	unlock(&c->lock);
}

void counter_decrement(counter_t *c)
{
	alock(&c->lock);
	c->value--;
	unlock(&c->lock);
}

int counter_get_value(counter_t *c)
{
	alock(&c->lock);
	int rc = c->value;
	unlock(&c->lock);
	return rc;
}
