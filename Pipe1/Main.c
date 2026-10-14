#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {

    //file descriptor df[0]--> read  fd[1]-->write
    int fd[2];
    if (pipe(fd)==-1) {
        perror("Pipe error");
        exit(-1);
    }

    //creazione nuovo processo
    pid_t ritorno_fork=fork();

    if(ritorno_fork==-1) {
        perror("fork");
        exit(-2);
    }

    //processo figlio

    if (!ritorno_fork) {
        close(fd[0]);
        char msg[]="Ciao padre";
        write(fd[1],msg,sizeof(msg));
        close(fd[1]);
        exit(0);
    }

    close(fd[1]);
    char buffer[100];

    read(fd[0],buffer,sizeof(buffer));

    close(fd[0]);
    printf("Messaggio dal figlio:%s\n",buffer);
    return 0;
}