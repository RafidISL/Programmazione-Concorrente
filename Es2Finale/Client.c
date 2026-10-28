
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_PATH "/tmp/echo_fifo"

int main() {
    int fd;
    char buf[512];

    signal(SIGPIPE, SIG_IGN);

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }

        if (write(fd, buf, strlen(buf)) == -1) {
            perror("write");
            close(fd);
            return 1;
        }

        close(fd);

        if (strcmp(buf, "QUIT\n") == 0 || strcmp(buf, "QUIT") == 0) {
            break;
        }
    }

    return 0;
}
