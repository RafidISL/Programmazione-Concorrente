#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define FIFO_PATH "/tmp/mia_fifo"

int main(void) {
    /* Crea la FIFO. Se esiste gia', non e' un errore. */
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Errore mkfifo");
            exit(EXIT_FAILURE);
        }
        /* errno == EEXIST: la FIFO c'e' gia', va bene cosi' */
    }
    printf("Mia fifo esiste")

    return 0;
}