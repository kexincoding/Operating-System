#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include "counter.h"
#define num (2)


counter_t c;
void * mythread(void *arg)
{
	int i;
	for (i = 0; i < 1e6; i++)
	{
		counter_increment(&c);
	}
	return NULL;
}


int main()
{
        pthread_t p[num];
	int j,k;
	struct timeval start,end;
	long costtime = 0;
	gettimeofday(&start,NULL);
	counter_init(&c, 0);
	for(j=0;j<num;j++)
	{
		pthread_create(&p[j], NULL, mythread, NULL);
		//pthread_join(p[j], NULL);
	}
	for(k=0;k<num;k++)
	{
		pthread_join(p[k], NULL);
	}
	gettimeofday(&end,NULL);
	costtime=1000000*(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
	printf("cost time:%ld\n",costtime);
	return 0;
}

