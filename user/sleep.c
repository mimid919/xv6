#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char *errorMessage = "No time is provided\n";


    //error message if no time provided - just types sleep
    if (argc < 2) {
        write(2, errorMessage, strlen(errorMessage));
        exit(1);
    }

    //convert the time argument from string to integer
    int time = atoi(argv[1]);

    sleep(time * 10);
    
    exit(0);
}
