#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    char *errorMessage = "Usage: findppid <num>\n";

    // print error to standard error if no number of child processes provided
    if (argc < 2) {
        write(2, errorMessage, strlen(errorMessage));
        exit(1);
    }

    // convert number of child processes from string to integer
    int numChildren = atoi(argv[1]);

    // plan is to make a loop
    for (int i = 0; i < numChildren; i++) {
        int pid = fork();
        if (pid > 0) { // parent process
            wait(0); // wait for child to finish
            // print the parent process ID
            printf("my ID is %d, my parent ID is %d\n", pid, getppid());
            exit(0);
        }
    }

    //eldest child prints
    printf("my ID is %d, my parent ID is %d\n", getpid(), getppid());

  
  exit(0);
}