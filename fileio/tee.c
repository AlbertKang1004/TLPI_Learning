#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "../lib/tlpi_hdr.h"

#define MAX_BUFFER 1024

extern int optind;

int main(int argc, char *argv[]) {
    int fd, opt;
    ssize_t numRead, numWritten;
    Boolean append_mode = FALSE;
    char buf[MAX_BUFFER];
    
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("| {%s <filename>|%s -a <filename>}\n", argv[0], argv[0]);
    
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
            case 'a':
                append_mode = TRUE;
                break;
            default:
                append_mode = FALSE;
        }
    }

    fd = open(argv[optind] , O_RDWR | O_CREAT | (append_mode ? O_APPEND : O_TRUNC), 
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1) 
        errExit("open");

    while ((numRead = read(STDIN_FILENO, buf, MAX_BUFFER)) > 0) {
        write(STDOUT_FILENO, buf, numRead);
        numWritten = write(fd, buf, numRead);
        if (numWritten == -1 || numWritten < numRead) 
            errExit("write");
    }

    if (close(fd) == -1)
        errExit("close");
    
    exit(EXIT_SUCCESS);
}