#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char *errorMessage = "Usage: sleep <ticks>\n";


    //error message if no time provided - just types sleep
    if (argc < 2) {
        write(2, errorMessage, strlen(errorMessage));
        exit(1);
    }

    //convert the time argument from string to integer
    int ticks = atoi(argv[1]);

    sleep(ticks);
    
    exit(0);
}
