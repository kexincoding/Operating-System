#include "list.h"

void list_init(list_t *list)
{
    list->head = NULL;
    init(&list->lock);
}

void list_insert(list_t *list, unsigned int key)
{
    node_t * new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        return;
    }
    new->key = key;
    spinlock_acquire(&list->lock);
    new->next = list->head;
    list->head = new;
    spinlock_release(&list->lock);
}

void list_delete(list_t *list, unsigned int key)
{
    int rv = -1;
    spinlock_acquire(&list->lock);
    node_t *curr = list->head,*last=NULL;
    while(1)
    {
        curr = list->head;
        last=NULL;
        while (curr)
        {
            if (curr->key == key)
            {
                rv = 0;
                break;
            }
            last = curr;
            curr = curr->next;
        }
        if(rv==-1)
        {
            break;
        }

        else if(last==NULL)
        {
            list->head=curr->next;
            if(curr->next==NULL)
            {
                free(curr);
                break;
            }
            else
                free(curr);
        }
        else
        {
            last->next=curr->next;
            if(curr->next==NULL)
            {
                free(curr);
                break;
            }
            else
                free(curr);
        }
    }
    spinlock_release(&list->lock);
}

void *list_lookup(list_t *list, unsigned int key)
{
    int rv =-1;
    spinlock_acquire(&list->lock);
    node_t * curr = list->head;
    while (curr)
    {
        if (curr->key == key)
        {
            rv=0;//printf("lookup:%d\n",(list->head)->key);
            break;

        }
        curr = curr->next;
    }
    spinlock_release(&list->lock);
    if(rv==-1)
    {
        //printf("NULL\n");
        return NULL;
    }
    else
        return curr;
}


