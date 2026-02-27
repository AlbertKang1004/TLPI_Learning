#include <sys/wait.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]) {
    int numChildren, j;
    pid_t childPid;

    int *shared_data = mmap(NULL, sizeof(int) * 3, PROT_READ | PROT_WRITE, 
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    int *measure = &shared_data[0];
    int *parentFirst = &shared_data[1];
    int *childFirst = &shared_data[2];

    *measure = 0; *parentFirst = 0; *childFirst = 0;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [num-children]\n", argv[0]);
    
    numChildren = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;
    setbuf(stdout, NULL);

    for (j = 0 ; j < numChildren; j++) {
        switch (childPid = fork()) {
            case -1:
                errExit("fork");
            case 0:
                printf("%d child\n", j);
                if (j == *measure) {
                    (*measure)++, (*childFirst)++;
                }
                _exit(EXIT_SUCCESS);
            default:
                printf("%d parent\n", j);
                if (j == *measure) {
                    (*measure)++, (*parentFirst)++;
                }
                wait(NULL); /* Wait for child to terminate */
                break;
        }
    }

    printf("Child won %d, Parent won %d\n", *childFirst, *parentFirst);
    munmap(shared_data, sizeof(int) * 3);
    exit(EXIT_SUCCESS);
}
