
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_AB "/tmp/chat_ab"
#define FIFO_BA "/tmp/chat_ba"

int main() {
    int fd_read, fd_write;
    char buf[512];
    ssize_t n;

    fd_read = open(FIFO_AB, O_RDONLY);
    if (fd_read == -1) {
        perror("open");
        return 1;
    }

    fd_write = open(FIFO_BA, O_WRONLY);
    if (fd_write == -1) {
        perror("open");
        close(fd_read);
        return 1;
    }

    while (1) {
        n = read(fd_read, buf, sizeof(buf) - 1);
        if (n <= 0) {
            break;
        }

        buf[n] = '\0';
        printf("A: %s", buf);
        fflush(stdout);

        if (strcmp(buf, "EXIT\n") == 0 || strcmp(buf, "EXIT") == 0) {
            break;
        }

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        if (write(fd_write, buf, strlen(buf) + 1) == -1) {
            perror("write");
            break;
        }

        if (strcmp(buf, "EXIT\n") == 0 || strcmp(buf, "EXIT") == 0) {
            break;
        }
    }

    close(fd_read);
    close(fd_write);

    return 0;
}
