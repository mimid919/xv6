#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    char *errorMessageNoNum = "Usage: primes <num>\n";
    char *errorMessageTooLarge = "for primes <num>, num must be between 2 and 200\n";


    // print error to standard error if no number of child processes provided
    if (argc < 2) {
        write(2, errorMessageNoNum, strlen(errorMessageNoNum));
        exit(1);
    } 

    // print error to standard error if number > 200
    int num = atoi(argv[1]);
    if (num > 200 || num < 2) {
        write(2, errorMessageTooLarge, strlen(errorMessageTooLarge));
        exit(1);
    }


    // create first pipe for generated natural numbers
    float input_pipe[2];
    pipe(input_pipe);

    // make an array with nums from 2 to num
    for (int i = 2; i <= num; i++) {
        write(input_pipe[1], &i, sizeof(i));
    }

    close(input_pipe[1]); // close the write end of the input pipe

    int pid;
    int prime = 2;

    while (read(input_pipe[0], &prime, sizeof(prime)) > 0) {
   
    
        // (b) print that prime
        printf("prime %d\n", prime);

        // (c) set up a new pipe for the next filter
        int right_pipe[2];
        pipe(right_pipe);

        // fork a child for the next filter
        pid = fork();
        if (pid == 0){
            // ----- now in the filter process 

            // close unused end
            close(right_pipe[1]); 
            close(input_pipe[0]);
            input_pipe[0] = right_pipe[0]; // the child will read from the new pipe

        } else {
            close(right_pipe[0]); // close the read end of the new pipe
            // parent will write to new pipe
            int num;
            while (read(input_pipe[0], &num, sizeof(num)) > 0) {
                if (num % prime != 0) {
                    write(right_pipe[1], &num, sizeof(num));
                }
            }

             // close both ends of the pipe
            close(right_pipe[1]); 
            close(input_pipe[0]);

            wait(0); // wait for child to finish
            exit(0);
        }
        




    }
    //for the last filter, there is no child process to wait for, so we just close the input pipe and exit
    close(input_pipe[0]); // tclose the read end of the input pipe
    exit(0);

}