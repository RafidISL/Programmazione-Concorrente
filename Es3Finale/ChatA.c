
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_AB "/tmp/chat_ab"
#define FIFO_BA "/tmp/chat_ba"

int main() {
    int fd_write, fd_read;
    char buf[512];
    ssize_t n;

    if (mkfifo(FIFO_AB, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    if (mkfifo(FIFO_BA, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    fd_write = open(FIFO_AB, O_WRONLY);
    if (fd_write == -1) {
        perror("open");
        return 1;
    }

    fd_read = open(FIFO_BA, O_RDONLY);
    if (fd_read == -1) {
        perror("open");
        close(fd_write);
        return 1;
    }

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (write(fd_write, buf, strlen(buf) + 1) == -1) {
            perror("write");
            break;
        }

        if (strcmp(buf, "EXIT\n") == 0 || strcmp(buf, "EXIT") == 0) {
            break;
        }

        n = read(fd_read, buf, sizeof(buf) - 1);
        if (n <= 0) {
            break;
        }

        buf[n] = '\0';
        printf("B: %s", buf);
        fflush(stdout);

        if (strcmp(buf, "EXIT\n") == 0 || strcmp(buf, "EXIT") == 0) {
            break;
        }
    }

    close(fd_write);
    close(fd_read);
    unlink(FIFO_AB);
    unlink(FIFO_BA);

    return 0;
}
