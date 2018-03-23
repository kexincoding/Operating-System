#include "mutex_list.h"

void list_init(list_t *list)
{
    list->head = NULL;
    mutex_init(&list->lock);
}

void list_insert(list_t *list, unsigned int key)
{
    mutex_acquire(&list->lock);
    node_t * new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        return;
    }
    new->key = key;

    new->next = list->head;
    list->head = new;
    mutex_release(&list->lock);
}

void list_delete(list_t *list, unsigned int key)
{
    int rv = -1;
    mutex_acquire(&list->lock);
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
    mutex_release(&list->lock);
}

void *list_lookup(list_t *list, unsigned int key)
{
    int rv =-1;
    mutex_acquire(&list->lock);
    node_t * curr = list->head;
    while (curr)
    {
        if (curr->key == key)
        {
            rv=0;
            break;
        }
        curr = curr->next;
    }
    mutex_release(&list->lock);
    if(rv==-1)
        return NULL;
    else
        return curr;
}
