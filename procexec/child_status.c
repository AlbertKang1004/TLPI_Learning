#define _XOPEN_SOURCE 500
#include <sys/wait.h>
#include "print_wait_status.h"
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]) {
    siginfo_t info;
    pid_t ret;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [exit-status]\n", argv[0]);
 
    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child started with PID = %ld\n", (long) getpid());
        if (argc > 1)
            exit(getInt(argv[1], 0, "exit-status"));
        else
            for (;;)
                pause();
        exit(EXIT_FAILURE);
    default:
        for (;;) {
            ret = waitid(P_ALL, 0, &info, WEXITED | WSTOPPED 
#ifdef WCONTINUED
                | WCONTINUED
#endif
                );
            if (ret == -1)
                errExit("waitid");
            
            printf("waitid() returned: CODE=%d; PID=%ld; SIGNO=%d; STATUS=%d; UID=%d\n", 
            info.si_code, (long) info.si_pid, info.si_signo, info.si_status, info.si_uid);

            if (info.si_code == CLD_EXITED || info.si_code == CLD_KILLED || info.si_code == CLD_DUMPED)
                exit(EXIT_SUCCESS);
        }
    }
}