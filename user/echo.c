#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    // start from index 1 because index 0 is the command name "echo"
    // write 1 (stdout), the argument string, and its length
    write(1, argv[i], strlen(argv[i]));
    // output a space after each argument except the last one
    // which should be followed by a newlin
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
