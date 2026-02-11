#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../lib/error_functions.h"

#ifndef MAX_BUFFER
#define MAX_BUFFER 100
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd;
    ssize_t numRead, numWrite;
    char buf[MAX_BUFFER];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) 
        usageErr("%s old-file new-file\n", argv[0]);

    inputFd = open(argv[1], O_RDWR);
    outputFd = open(argv[2], O_CREAT | O_WRONLY, 0644);
    if (inputFd == -1 || outputFd == -1)
        errExit("open");

    if (lseek(inputFd, 3, SEEK_SET) == -1) 
        errExit("lseek");

    numRead = read(inputFd, buf, MAX_BUFFER);
    if (numRead == -1) 
        errExit("read");

    if (lseek(outputFd, 3, SEEK_SET) == -1) 
        errExit("lseek");

    numWrite = write(outputFd, buf, numRead);
    if (numWrite != numRead)
        fatal("couldn't write whole buffer (partial)");

    if (numWrite == -1) 
        errExit("write");

    if (close(inputFd) == -1) errExit("close input");
    if (close(outputFd) == -1) errExit("close output");

    return 0;
}