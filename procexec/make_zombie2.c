#define _XOPEN_SOURCE 500
#include <signal.h>
#include <libgen.h>
#include <stdio.h>
#include <signal.h>
#include "../lib/tlpi_hdr.h"

#define CMD_SIZE 200

int main(int argc, char *argv[]) {
    char cmd[CMD_SIZE];
    pid_t childPid;

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long) getpid());

    switch(childPid = fork())  {
    case -1:
        errExit("fork");
    case 0:
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        kill(getppid(), SIGUSR1);
        _exit(EXIT_SUCCESS);
    default:
        int sig;
        if (sigwait(&set, &sig) != 0)
            errExit("sigwait");
        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
        cmd[CMD_SIZE - 1] = '\0';
        system(cmd);

        if (kill(childPid, SIGKILL) == -1) 
            errMsg("kill");
        sleep(3);
        printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
        system(cmd);

        exit(EXIT_SUCCESS);
    }
    return 0;
}
