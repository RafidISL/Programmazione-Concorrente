#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) < 0) {
        perror("Errore pipe1");
        exit(1);
    }

    if (pipe(pipe2) < 0) {
        perror("Errore pipe2");
        exit(1);
    }

    pid_t B = fork();

    if (B < 0) {
        perror("Errore fork B");
        exit(1);
    }

    if (B == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        int n;

        while (read(pipe1[0], &n, sizeof(n)) == sizeof(n)) {
            if (n >= 50) {
                int quadrato = n * n;
                write(pipe2[1], &quadrato, sizeof(quadrato));
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    pid_t C = fork();

    if (C < 0) {
        perror("Errore fork C");
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe1[0]);
        close(pipe2[1]);
        wait(NULL);
        exit(1);
    }

    if (C == 0) {
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        int n;
        int somma = 0;
        int contatore = 0;

        while (read(pipe2[0], &n, sizeof(n)) == sizeof(n)) {
            printf("Quadrato ricevuto: %d\n", n);
            somma += n;
            contatore++;
        }

        if (contatore > 0) {
            printf("Media aritmetica: %.2f\n",
                   (double)somma / contatore);
        } else {
            printf("Nessun numero ricevuto.\n");
        }

        close(pipe2[0]);

        exit(0);
    }

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);

    srand(time(NULL));

    for (int i = 0; i < 20; i++) {
        int n = rand() % 100 + 1;

        printf("Padre A genera: %d\n", n);
        write(pipe1[1], &n, sizeof(n));
    }

    close(pipe1[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
