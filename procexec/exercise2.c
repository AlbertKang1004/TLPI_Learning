#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t processID;
    int status;

    switch (processID = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0: 
        switch (processID = fork()) { // creates a grandchild process
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            sleep(1); // time between the parent's _exit call and wait call by grandparent
            printf("After the parent terminates, the PID of parent is %d.\n", getppid());
            sleep(2);
            printf("After the grandparent calls wait, the PID of parent is %d.\n", getppid());
            _exit(EXIT_SUCCESS);
        default:
            _exit(EXIT_SUCCESS); // this happens first
        }
    default:
        sleep(2); // Wait until the child process exits successfully
        wait(&status); // store termination status
        printf("Grandparent Process with PID = %ld has been successfully terminated with code %d\n", (long) processID, status);
    }
    return 0;
}
