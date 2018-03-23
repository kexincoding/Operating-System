#include "mem.h"
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/mman.h>

typedef struct
{
    int size;
    int magic;
} header;

typedef struct node
{
    int size;
    struct node *next;
} node;

int m_error;
node *head=NULL;
int flag=0;

int mem_init(int size_of_region)
{
    if(flag==1)
        return -1;
    int fd;
    fd=open("/dev/zero",O_RDWR);
    void *ptr=mmap(NULL,size_of_region,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
    if(ptr==MAP_FAILED)
    {
        m_error=E_BAD_ARGS;
        return -1;
    }
    close(fd);
    node *m;
    m=ptr;
    if(size_of_region%4096!=0)
        size_of_region=((size_of_region >> 12) + 1) << 12;
    m->size=size_of_region-sizeof(node);
    m->next=NULL;
    head=m;
    flag=1;
    return 0;
}

void * mem_alloc(int size, int style)
{
    header *p,*h;
    node *b= head,*last= head,*x,*next1=NULL,*l=NULL,*now;
    int s,s1,nowsize=0;
    if(size%8!=0)
        s1=((size >> 3) + 1) << 3;
    else
        s1=size;
    s=s1+sizeof(header);
    if(style==0)
    {
        int min=100000;
        while(b)
        {
            if(min>b->size+sizeof(node)-s&&b->size+sizeof(node)-s>=0)
            {
                min=b->size+sizeof(node)-s;
                l=last;
                now=b;
                nowsize=now->size;
                next1=now->next;
            }
            last = b;
            b = b->next;
        }
        if(l==NULL)
        {
            m_error=E_NO_SPACE;
            return NULL;
        }
    }
    if(style==1)
    {
        int max=0;
        while(b)
        {
            if(b->size>max)
            {
                max=b->size;
                l=last;
                now=b;
                nowsize=now->size;
                next1=now->next;
            }
            last = b;
            b = b->next;
        }
        if(max+sizeof(node)<s)
        {
            m_error=E_NO_SPACE;
            return NULL;
        }
    }
    if(style==2)
    {
        while(b !=NULL && (b->size+sizeof(node))<s)
        {
            last = b;
            b = b->next;
        }
        if(b==NULL)
        {
            m_error=E_NO_SPACE;
            return NULL;
        }
        nowsize=b->size;
        next1=b->next;
        l=last;
        now=b;
    }
    h=(header*)now;
    if(now->size-s>=0)
    {
        h->size=s1;
        h->magic=1234567;
        p=(void*)h+sizeof(header);
        x=(void*)p+s1;
        if(now!=head)
            l->next=(node*)x;
        else
            head=x;

        x->size=nowsize-s1-sizeof(header);
        x->next=next1;
    }
    else
    {
        h->size=now->size+sizeof(node)-sizeof(header);
        h->magic=1234567;
        p=(void*)h+sizeof(header);
        if(now==head)
            head=next1;
        else
            l->next=next1;
    }
    return p;
}

int mem_free(void *ptr)
{
    if(ptr==NULL)
        return 0;
    header *hptr;
    node *b=head,*last=NULL,*x;
    int num;
    hptr=ptr-sizeof(header);
    num=hptr->size+sizeof(header);
    if(hptr->magic==1234567)
    {
        if(head==NULL)
        {
            x=(void*)hptr;
            x->size=num-sizeof(node);
            x->next=NULL;
            head=x;
            return 0;
        }
        while((void*)b<(void*)hptr&&b!=NULL)
        {
            last = (node*)b;
            b = b->next;
        }
        if(b==NULL&&(void*)last+last->size+sizeof(node)==(void*)hptr)
            last->size=last->size+num;
        else if(b==NULL&&(void*)last+last->size+sizeof(node)!=(void*)hptr)
        {
            x=(void*)hptr;
            if(b==head)
            {
                head=x;
            }
            else
                last->next=x;
            x->size=num-sizeof(node);
            x->next=b;
        }
        else if(last&&(void*)last+last->size+sizeof(node)==(void*)hptr&&b!=(void*)hptr+num)
        {
            last->size=last->size+num;
        }
        else if(last&&(void*)last+last->size+sizeof(node)==(void*)hptr&&b==(void*)hptr+num)
        {
            last->size=last->size+num+b->size+sizeof(header);
            last->next=b->next;
        }
        else if(last&&(void*)last+last->size+sizeof(node)!=(void*)hptr&&b==(void*)hptr+num)
        {
            x=(void*)hptr;
            if(b==head)
            {
                head=x;
            }
            else
                last->next=x;
            x->size=num+b->size;
            x->next=b->next;
        }
        else if(last==NULL&&b==(void*)hptr+num)
        {
            x=(void*)hptr;
            if(b==head)
            {
                head=x;
            }
            else
                last->next=x;
            x->size=num+b->size;
            x->next=b->next;
        }
        else
        {
            x=(void*)hptr;
            if(b==head)
            {
                head=x;
            }
            else
                last->next=x;
            x->size=num-sizeof(node);
            x->next=b;
        }
    }
    else
    {
        m_error=E_BAD_POINTER;
        return -1;
    }
    return 0;
}

void mem_dump()
{
    node *t;
    t=head;
    while(t!=NULL)
    {
        printf("%p\n",t);
        printf("%d\n",t->size);
        t=t->next;
    }
    printf("\n\n");
}
