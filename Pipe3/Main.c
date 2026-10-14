#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

struct Messaggio {
    char testo[100];
    int vocali;
};

int main(void) {
    int fd1[2];
    int fd2[2];

    if (pipe(fd1) < 0) {
        perror("Error pipe1");
        exit(-1);
    }

    if (pipe(fd2) < 0) {
        perror("Error pipe2");
        exit(-1);
    }

    pid_t ritorno = fork();

    if (ritorno < 0) {
        perror("Error fork");
        exit(-2);
    }

    if (!ritorno) {
        close(fd1[1]);
        char testo[100];

        read(fd1[0], testo, sizeof(testo));

        close(fd1[0]);
        close(fd2[0]);

        for (int i = 0; testo[i] != '\0'; i++) {
            testo[i] = toupper((unsigned char)testo[i]);
        }

        int vocali = 0;

        for (int i = 0; testo[i] != '\0'; i++) {
            if (testo[i] == 'A' ||
                testo[i] == 'E' ||
                testo[i] == 'I' ||
                testo[i] == 'O' ||
                testo[i] == 'U') {
                vocali++;
                }
        }

        struct Messaggio msg;
        strcpy(msg.testo, testo);
        msg.vocali = vocali;

        write(fd2[1], &msg, sizeof(msg));

        close(fd2[1]);

    } else {
        close(fd1[0]); //legge da fd2 quindi la lettura in fd1 non serve
        close(fd2[1]);//scrive da fd1 quindi la scrittura in fd2 non serve

        char testo[100];

        printf("Inserisci una stringa: ");
        fgets(testo, sizeof(testo), stdin);

        testo[strcspn(testo, "\n")] = '\0';

        write(fd1[1], testo, strlen(testo) + 1);

        close(fd1[1]);

        struct Messaggio msg;

        read(fd2[0], &msg, sizeof(msg));

        close(fd2[0]);

        wait(NULL);

        printf("Stringa maiuscola: %s\n", msg.testo);
        printf("Numero di vocali: %d\n", msg.vocali);
    }

    return 0;
}
