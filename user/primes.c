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

    int pipe_fd[2];
    pipe(pipe_fd);

    int prime;
    int pid;

    pid = fork();
    if (pid == 0) {
        close(pipe_fd[1]);
        int input = pipe_fd[0];

        while (read(input, &prime, sizeof(prime)) > 0) {
            printf("%d\n", prime);

            int next_pipe[2];
            pipe(next_pipe);

            int child_pid = fork();
            if (child_pid == 0) {
                close(next_pipe[1]);
                close(input);
                input = next_pipe[0];
            } else {
                close(next_pipe[0]);
                int num;
                while (read(input, &num, sizeof(num)) > 0) {
                    if (num % prime != 0) {
                        write(next_pipe[1], &num, sizeof(num));
                    }
                }
                close(next_pipe[1]);
                close(input);
                wait(0);
                exit(0);
            }
        }
        close(input);
        exit(0);
    } else {
        close(pipe_fd[0]);
        for (int i = 2; i <= n; i++) {
            write(pipe_fd[1], &i, sizeof(i));
        }
        close(pipe_fd[1]);
        wait(0);
    }

    exit(0);
}