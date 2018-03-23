#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


int redirection(char** a,int k);
int redirection1(char** a,int k,int fd[][2],int j);
int redirectionandback(char** a,int k);
int backrgound(char** a,int k);
int background1(char** a,int k,int fd[][2],int j);
int execute(char* order,int k);
int pipes(char *order);


int redirection(char** a,int k)
{
    char error_message[30] = "An error has occurred\n";
    int fd,i;
    for(i=0; i<k; i++)
    {
        if(strcmp(a[i],">")==0)
        {
            if(a[i+1]==NULL||a[i+2]!=NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                return 0;
            }
            int rc=fork();
            if(rc<0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if(rc==0)
            {
                fd=open(a[i+1],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
                if(fd==-1)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    return 0;
                }
                close(1);
                dup2(fd,1);
                close(fd);
                a[i]=NULL;
                execvp(a[0],a);
            }
            else
                waitpid(rc,0,0);
        }
    }
    return 0;
}




int redirection1(char** a,int k,int fd[][2],int j)
{
    char error_message[30] = "An error has occurred\n";
    int f,i;
    for(i=0; i<k; i++)
    {
        if(strcmp(a[i],">")==0)
        {
            if(a[i+1]==NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                return 0;
            }
            int rc=fork();
            if(rc<0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if(rc==0)
            {
		close(fd[j-1][1]);
                dup2(fd[j-1][0], STDIN_FILENO);
                close(fd[j-1][0]);
                f=open(a[i+1],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
                if(f==-1)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    return 0;
                }
                close(1);
                dup2(f,1);
                a[i]=NULL;
                execvp(a[0],a);

            }
            else
                waitpid(rc,0,0);
        }
    }
    return 0;
}


int redirectionandback(char** a,int k)
{
    char error_message[30] = "An error has occurred\n";

    int fd,i;
    for(i=0; i<k; i++)
    {
        if(strcmp(a[i],">")==0)
        {
            if(a[i+1]==NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                return 0;
            }
            int rc=fork();
            if(rc<0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if(rc==0)
            {
                fd=open(a[i+1],O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
                if(fd==-1)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    return 0;
                }
                close(1);
                dup2(fd,1);
                close(fd);
                a[i]=NULL;
                execvp(a[0],a);
            }
        }
    }
    return 0;
}




int background(char** a,int k)
{
    char error_message[30] = "An error has occurred\n";
    int i;
    for(i=0; i<k; i++)
    {
        if(strcmp(a[i],"&")==0)
        {
            a[i]=NULL;
            int rc=fork();
            if(rc<0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if(rc==0)
            {
                execvp(a[0],a);
            }
        }
    }
    return 0;
}



int background1(char** a,int k,int fd[][2],int j)
{
    char error_message[30] = "An error has occurred\n";
    int i;
    for(i=0; i<k; i++)
    {
        if(strcmp(a[i],"&")==0)
        {
            a[i]=NULL;
            int rc=fork();
            if(rc<0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if(rc==0)
            {
                close(fd[j-1][1]);
                dup2(fd[j-1][0], STDIN_FILENO);
                close(fd[j-1][0]);
                execvp(a[0],a);
            }
        }
    }
    return 0;
}


int execute(char* order,int k)
{
    char* a[200],neworder[1000],len,i,m;
    char error_message[30] = "An error has occurred\n";
    if(strlen(order)>512)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return 0;
    }
    memset(neworder,0,1000*sizeof(char));
    len=strlen(order);
    strcpy(neworder,order);
    for(i=0; i<len; i++)
    {
        if(order[i]=='>'||order[i]=='&')
       {
            for(m=len+2; m>i; m--)
                order[m]=order[m-2];
            order[i+1]=order[i];
            order[i]=' ';
            order[i+2]=' ';
	    ++i;
            len=len+2;
       }
    }

    a[k]=strtok(order," ");
    while(a[k])
    {
        a[++k]=strtok(NULL," ");
    }
    a[k]=strtok(NULL,"\n");
    a[k+1]=NULL;

    if(a[0]==NULL)
        return 0;

    if(strcmp(a[0],"exit")==0&&a[1]==NULL)
        exit(0);
    else if(strcmp(a[0],"exit")==0&&a[1]!=NULL)
        write(STDERR_FILENO, error_message, strlen(error_message));
    else if(strcmp(a[0],"pwd")==0)
    {
        char *buf;
        buf=getcwd(NULL,0);
        strcat(buf,"\n");
        write(STDOUT_FILENO, buf, strlen(buf));
    }
    else if(strcmp(a[0],"cd")==0&&a[1]==NULL)
    {
        chdir(getenv("HOME"));
    }
    else if(strcmp(a[0],"cd")==0&&a[1]!=NULL)
    {
        if((chdir(a[1]))!=0)
            write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else if(strcmp(a[0],"wait")==0&&a[1]==NULL)
        while(wait(0)>0);
    else if(strcmp(a[0],"wait")==0&&a[1]!=NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return 0;
    }


    else if(strchr(neworder,'>')!=NULL&&strchr(neworder,'&')==NULL)
        redirection(a,k);


    else if(strchr(neworder,'&')!=NULL&&strchr(neworder,'>')==NULL)
        background(a,k);

    else if(strchr(neworder,'>')!=NULL&&strchr(neworder,'&')!=NULL)
        redirectionandback(a,k);

    else
    {
        int rc=fork();
        if(rc<0)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        else if(rc==0)
        {
            if(execvp(a[0],a)==-1)
                write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        else
            waitpid(rc,0,0);
    }

}



int pipes(char *order)
{
    int k,n,m=0,len,fd[100][2],i,pid[100],j=0,flag=0,x,y;
    char *o[100],*a[200],neworder[1000];
    char error_message[30] = "An error has occurred\n";
    m = 0;
    o[m] = strtok(order, "|");
    while (o[m])
    {
        m++;
        o[m] = strtok(NULL, "|");
    }
    for (j = 0; j < m; j++)
    {
        for (n = 0; n < k; ++n)
        {
            a[n] = NULL;
        }
        memset(neworder,0,1000*sizeof(char));
		strcpy(neworder,o[j]);
        len=strlen(o[j]);
        for(x=0; x<len; x++)
        {
            if(o[j][x]=='>'||o[j][x]=='&')
			{
                for(y=len+2; y>x; y--)
                    o[j][y]=o[j][y-2];
                o[j][x+1]=o[j][x];
                o[j][x]=' ';
                o[j][x+2]=' ';
		++x;
		len=len+2;
            }
        }

        k= 0;
        a[k] = strtok(o[j], " ");
        while (a[k])
        {
            k++;
            a[k] = strtok(NULL, " ");
        }




        if(strcmp(a[0],"exit")==0&&a[1]==NULL)
            exit(0);
        else if(strcmp(a[0],"exit")==0&&a[1]!=NULL)
            write(STDERR_FILENO, error_message, strlen(error_message));
        else if(strcmp(a[0],"pwd")==0)
        {
            char *buf;
            buf=getcwd(NULL,0);
            strcat(buf,"\n");
            write(STDOUT_FILENO, buf, strlen(buf));
        }
        else if(strcmp(a[0],"cd")==0&&a[1]==NULL)
        {
            chdir(getenv("HOME"));
        }
        else if(strcmp(a[0],"cd")==0&&a[1]!=NULL)
        {
            if((chdir(a[1]))!=0)
                write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else if(strcmp(a[0],"wait")==0&&a[1]==NULL)
            while(wait(0)>0);
        else if(strcmp(a[0],"wait")==0&&a[1]!=NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return 0;
        }


        else if(strchr(neworder,'>')!=NULL&&j!=m-1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return 0;
        }

        else if(strchr(neworder,'>')!=NULL&&j==m-1)
        {
		redirection1(a,k,fd,j);
            return 0;
        }


        else if(strchr(neworder,'&')!=NULL&&j!=m-1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return 0;
        }
        else if(strchr(neworder,'&')!=NULL&&j==m-1)
        {
            background1(a,k,fd,j);
            return 0;
        }


        if (j < m - 1)
            pipe(fd[j]);

        pid[j] = fork();
        if (pid[j] < 0)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        }
        else if (pid[j] == 0)
        {
            if (m > 1)
            {
                if (j == 0)
                {
                    close(fd[j][0]);
                    dup2(fd[j][1], STDOUT_FILENO);
                    close(fd[j][1]);
                }
                else if (j == m - 1)
                {
                    close(fd[j-1][1]);
                    dup2(fd[j-1][0], STDIN_FILENO);
                    close(fd[j-1][0]);
                }
                else
                {
                    dup2(fd[j-1][0], STDIN_FILENO);
                    close(fd[j-1][0]);
                    dup2(fd[j][1], STDOUT_FILENO);
                    close(fd[j][1]);
                }
            }
            execvp(a[0], a);
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        else

        {
            waitpid(pid[j], NULL, 0);
            if(j > 0) close(fd[j-1][0]);
            if(j < m-1) close(fd[j][1]);
        }

    }
    return 0;
}


int main(int argc, char** argv)
{
    int k,fd2,n,m=0,len,fd[100][2],i,pid[100],j=0;
    char order[1000],neworder[1000],*o[100];
    char *cmdline,*buf2,*p;


    if(argv[1]!=NULL)
    {
        fd2=open(argv[1],O_RDONLY);
        close(0);
        dup2(fd2,0);
        while(gets(order)!=NULL)
        {
            k=0;
            memset(neworder,0,1000*sizeof(char));
            write(STDOUT_FILENO, order, strlen(order));
            write(STDOUT_FILENO, "\n", strlen("\n"));
            if(strchr(order,'|')!=NULL)
                pipes(order);

            else
                execute(order,k);
        }
        exit(0);
    }

    else
    {
        while(1)
        {
            memset(neworder,0,1000*sizeof(char));
            k=0;
            cmdline="mysh> ";
            write(STDOUT_FILENO, cmdline, strlen(cmdline));
            gets(order);

            if(strchr(order,'|')!=NULL)
                pipes(order);

            else
                execute(order,k);
        }
    }
    return 0;
}

