
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PATH "/tmp/echo_fifo"

int main() {
    int fd;
    char buf[512];
    ssize_t n;

    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        unlink(FIFO_PATH);
        return 1;
    }

    n = read(fd, buf, sizeof(buf) - 1);
    if (n == -1) {
        perror("read");
        close(fd);
        unlink(FIFO_PATH);
        return 1;
    }

    buf[n] = '\0';
    printf("[SERVER] %s", buf);

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}
