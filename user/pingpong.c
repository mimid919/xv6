#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
// takes no arguments
main(void)
{
    // a pipe for each child
    int pipec1_to_p[2];
    int pipec2_to_p[2]; 
    int pipep_to_c1[2]; 
    int pipep_to_c2[2]; 

    
    pipe(pipec1_to_p);
    pipe(pipec2_to_p);
    pipe(pipep_to_c1);
    pipe(pipep_to_c2);

    char ping[] = "ping";
    char ACK[] = "ACK";

    // first fork
    int pid1 = fork();

    if (pid1 == 0){
        //child 1

        char buf[5];

        close(pipep_to_c1[1]);  //close the write 
        close(pipec1_to_p[0]);  //close the read

        read(pipep_to_c1[0], buf, sizeof(buf));

        printf("%d: pong\n", getpid());

        write(pipec1_to_p[1], ACK, sizeof(ACK));

        close(pipep_to_c1[0]);  //close the read 
        close(pipec1_to_p[1]);  //close the write

        exit(0);
    }

    // parent forks again
    int pid2 = fork();

    if (pid2 == 0){
        //child 2

        char buf[5];

        close(pipep_to_c2[1]);  //close the write 
        close(pipec2_to_p[0]);  //close the read

        read(pipep_to_c2[0], buf, sizeof(buf));

        printf("%d: pong\n", getpid());

        write(pipec2_to_p[1], ACK, sizeof(ACK));

        close(pipep_to_c2[0]);  //close the read 
        close(pipec2_to_p[1]);  //close the write

        exit(0);
    }

    //parent 
    close(pipec1_to_p[1]);
    close(pipec2_to_p[1]);
    close(pipep_to_c1[0]);
    close(pipep_to_c2[0]);

    //send ping
    write(pipep_to_c1[1], ping, sizeof(ping));
    write(pipep_to_c2[1], ping, sizeof(ping));

    close(pipep_to_c1[1]);
    close(pipep_to_c2[1]);


    char buf1[4];
    char buf2[4];

    read(pipec1_to_p[0], buf1, sizeof(buf1));
    read(pipec2_to_p[0], buf2, sizeof(buf2));

    close(pipec1_to_p[0]);
    close(pipec2_to_p[0]);

    wait(0);
    wait(0);

    exit(0);


}