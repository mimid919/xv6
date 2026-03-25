#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        write(2, "Usage: primes <num>\n", 20);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n < 2 || n > 200) {
        write(2, "for primes <num>, num must be between 2 and 200\n", 48);
        exit(1);
    }

    int input_pipe[2];
    pipe(input_pipe);

    for (int i = 2; i <= n; i++) {
        write(input_pipe[1], &i, sizeof(i));
    }
    close(input_pipe[1]);

    int prime;
    int pid;

    while (read(input_pipe[0], &prime, sizeof(prime)) > 0) {
        printf("%d\n", prime);

        int right_pipe[2];
        pipe(right_pipe);

        pid = fork();
        if (pid == 0) {
            close(right_pipe[1]);
            close(input_pipe[0]);
            input_pipe[0] = right_pipe[0];
        } else {
            close(right_pipe[0]);
            int num;
            while (read(input_pipe[0], &num, sizeof(num)) > 0) {
                if (num % prime != 0) {
                    write(right_pipe[1], &num, sizeof(num));
                }
            }
            close(right_pipe[1]);
            close(input_pipe[0]);
            wait(0);
            exit(0);
        }
    }

    close(input_pipe[0]);
    exit(0);
}