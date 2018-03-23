#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include "sort.h"


int cmp(const void* a, const void* b)
{
	return (long long)(*(rec_t*)a).key-(long long)(*(rec_t*)b).key;
}


int main(int argc, char** argv)
{
	if(argc!=3)
	{
		fprintf(stderr,"Usage: fastsort inputfile outputfile\n");
		exit(1);
	}

int fd1,fd2,i=0,m1,m2,k,p;
struct stat a;

if((fd1=open(argv[1],O_RDONLY))==-1)
{
	fprintf(stderr,"ERROR: cannot open %s \n",argv[1]);
	exit(1);
}

fstat(fd1,&a);
p=a.st_size/100;
rec_t *buf;

buf=(rec_t*)malloc(sizeof(rec_t)*p);

while(1)
{
	m1 = read(fd1, &buf[i], sizeof(rec_t));
	if (m1 == 0&&i==0) 
	{
	    fprintf(stderr,"ERROR: the file %s is empty\n",argv[1]);
	    exit(1);
	}
        if (m1 == 0&&i!=0) 
		break;
	if (m1 < 0) 
        {
	    fprintf(stderr,"ERROR: read error\n");
	    exit(1);
	}
	i++;
}

qsort(buf,i,sizeof(buf[0]),cmp);

if((fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC))==-1)
{
	fprintf(stderr,"ERROR: cannot open %s \n",argv[2]);
	exit(1);
}

for (k=0;k<i;k++)
{
	m2=write(fd2,&buf[k],sizeof(rec_t));
	if (m2 < 0) 
	{
	    fprintf(stderr,"ERROR: write error\n");
	    exit(1);
	}
}

free(buf);

(void)close(fd1);
(void)close(fd2);

return 0;
}

