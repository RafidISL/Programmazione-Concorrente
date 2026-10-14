#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <threads.h>
#include <sys/wait.h>


int main(void) {
    int numeri[]={1,2,3,4,5,6,7,8,9,10};
    int somma=0;


    int fd[2];
    if (pipe(fd)<0) {
        perror("Piper error");
        exit(-1);
    }

    pid_t ritorno_fork=fork();

    if (!ritorno_fork) {
        close (fd[0]);
        for (int i=0;i<10;i++){
            if (numeri[i]%2==0) {
                somma+=numeri[i];
            }
        }

        write(fd[1],&somma,sizeof(somma));
        close(fd[1]);
        exit(0);
    }else {
        close(fd[1]);
        int somma_pari;
        for (int i=0;i<10;i++) {
            if (numeri[i]%2!=0) {
                somma+=numeri[i];
            }
        }
        read(fd[0],&somma_pari,sizeof(somma_pari));
        close(fd[0]);
        wait(NULL);
        printf("Somma numeri pari: %d \n",somma_pari);
        printf("somma numeri dispari: %d \n",somma);
        printf("somma complessiva: %d \n",somma_pari+somma);





    }
    return 0;
}
