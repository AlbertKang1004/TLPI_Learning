#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

int main() {
    int *fd;
    fd = open("text.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {

    }
    return 0;
}
