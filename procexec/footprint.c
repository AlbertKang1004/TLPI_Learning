#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include "../lib/tlpi_hdr.h"

int main() {
    pid_t childPid;
    int status;

    childPid = fork();
    if (childPid == -1)
        errExit("fork");

    if (childPid == 0)
        exit(123);
    
    if (wait(&status) == -1)
        errExit("wait");

    printf("%d\n", WEXITSTATUS(status));
    return 0;
}

