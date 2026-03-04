#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
// #include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]) {
    pid_t processID;
    switch(processID = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        sleep(3); // making sure parent process ends earlier than the child process
        printf("This process is a orphan process.\n");
        printf("The parent of this process has a PID %ld.\n", (long) getppid());
        _exit(EXIT_SUCCESS);
    default:
        // note there's no wait call, so the child becomes an orphan process
        printf("Parent Process (%ld) terminates now.\n", (long) processID);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
