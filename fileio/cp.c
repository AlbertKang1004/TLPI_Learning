#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "../lib/tlpi_hdr.h"

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    int fd, fdw;
    ssize_t numRead, numWritten;
    char buf[MAX_BUFFER];

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s <src> <desc>\n", argv[1]);
    
    fd = open(argv[1], O_RDONLY);
    fdw = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    
    int flags = fcntl(fd, F_GETFL); // for debug
    int flags2 = fcntl(fdw, F_GETFL); // for debug
    if (fd == -1 || fdw == -1) 
        errExit("open");

    while ((numRead = read(fd, buf, MAX_BUFFER)) > 0) {
        numWritten = write(fdw, buf, numRead);
        if (numWritten == -1 || numWritten < numRead)
            errExit("write");
    }

    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}