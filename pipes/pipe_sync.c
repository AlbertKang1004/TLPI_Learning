#include <unistd.h>
#include <stdio.h>
#include "../lib/curr_time.h"
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]) {
    int pfd[2];
    int j, dummy;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time...\n", argv[0]);

    setbuf(stdout, NULL);
    printf("%s  Parent Started\n", currTime("%T"));

    if (pipe(pfd) == -1) 
        errExit("pipe");
    
    // process each arguments
    for (j = 1; j < argc; j++) {
        switch(fork()) {
        case -1:
            errExit("fork");
        case 0: // when it it a child process
            if (close(pfd[0]) == -1) // close read end (child)
                errExit("close - child");

            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));

            printf("%s  Child %d (PID=%ld) closing pipe\n", currTime("%T"), j,(long) getpid());

            if (close(pfd[1]) == -1) // close write end (child)
                errExit("close - child");
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    if (close(pfd[1]) == -1) // close write end (parent)
        errExit("close - parent");
    
    if (read(pfd[0], &dummy, 1) != 0)
        fatal("parent didn't get EOF");
    printf("%s  Parent ready to go\n", currTime("%T"));
    
    if (close(pfd[0]) == -1) // close read end (parent)
        errExit("close - parent");
    exit(EXIT_SUCCESS);
}

