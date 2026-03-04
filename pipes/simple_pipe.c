#include <sys/wait.h>
#include "../lib/tlpi_hdr.h"
#include <stdio.h>

#define BUF_SIZE 10

int main(int argc, char *argv[]) {
    int fd[2];
    char buf[BUF_SIZE];
    ssize_t numRead;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s string\n", argv[0]);

    if (pipe(fd) == -1) 
        errExit("pipe");

    switch(fork()) {
    case -1:
        errExit("fork");
    case 0:
        if (close(fd[1]) == -1) // closing write (child reads)
            errExit("close - child");
        
        for (;;) {
            numRead = read(fd[0], buf, BUF_SIZE); // read from parent
            if (numRead == -1)
                errExit("read");
            if (numRead == 0)
                break;
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                fatal("child - partial/failed write");
        }

        write(STDOUT_FILENO, "\n", 1);
        if (close(fd[0]) == -1) // closing read after finished reading from parent
            errExit("close");
        _exit(EXIT_SUCCESS);
    default:
        if (close(fd[0]) == -1) // closing read (parent writes)
            errExit("close - parent");
        if (write(fd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
            fatal("parent - partial/failed write");
        if (close(fd[1]) == -1) // closing write after finished writing to child
            errExit("close");
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
