#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(2, "Usage: primesum <num>\n", 22);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n < 2 || n > 200) {
        write(2, "for primesum <num>, num must be between 2 and 200\n", 49);
        exit(1);
    }

    int pipe_fd[2];
    pipe(pipe_fd);

    // Parent generates numbers 2..n
    if (fork() == 0) {
        close(pipe_fd[1]);       // close write end in child
        int input = pipe_fd[0];  // read from pipe

        int prev_prime = 0; // store previous prime for sum

        while (1) {
            int prime;
            int r = read(input, &prime, sizeof(prime));
            if (r <= 0) break; // no more numbers

            if (prev_prime > 0) {
                printf("%d\n", prev_prime + prime);
            }
            prev_prime = prime;

            // set up next pipe
            int next_pipe[2];
            pipe(next_pipe);

            if (fork() == 0) {
                // child: reads from next_pipe
                close(next_pipe[1]); 
                close(input);
                input = next_pipe[0];
            } else {
                // parent: filter remaining numbers
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
        // Parent writes numbers
        close(pipe_fd[0]);
        for (int i = 2; i <= n; i++) {
            write(pipe_fd[1], &i, sizeof(i));
        }
        close(pipe_fd[1]);
        wait(0);
    }

    exit(0);
}