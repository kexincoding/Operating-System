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
    alock(&list->lock);
    new->next = list->head;
    list->head = new;
    unlock(&list->lock);
    //printf("insert:%d\n",(list->head)->key);
}

void list_delete(list_t *list, unsigned int key)
{
    int rv = -1;
    alock(&list->lock);
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
		//printf("%d\n",key);
                break;
            }
            last = curr;
            curr = curr->next;
        }
        if(rv==-1)
        {
            //perror("delete Not Found");
	    //printf("1 %d\n",key);
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
    unlock(&list->lock);
}

void *list_lookup(list_t *list, unsigned int key)
{
    int rv =-1;
    alock(&list->lock);
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
    unlock(&list->lock);
    if(rv==-1)
        {
		//printf("NULL\n");		
                return NULL;
	}
    else
        return curr;
}


