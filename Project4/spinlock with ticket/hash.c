#include "hash.h"

void hash_init(hash_t *hash, int size)
{
	hash->lists=(list_t*)malloc(sizeof(list_t)*size);
	int i;
    hash->size = size;
    for (i = 0; i < size; i++)
    {
        list_init(&hash->lists[i]);
    }
}

void hash_insert(hash_t *hash, unsigned int key)
{
    int bucket = key % (hash->size);
    list_insert(&hash->lists[bucket], key);
}

void hash_delete(hash_t *hash, unsigned int key)
{
    int bucket = key % (hash->size);
    list_delete(&hash -> lists[bucket], key);
}

void *hash_lookup(hash_t *hash, unsigned int key)
{
    int bucket = key % (hash->size);
    return list_lookup(&(hash->lists[bucket]), key);
}
