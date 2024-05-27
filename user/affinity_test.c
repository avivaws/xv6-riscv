#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    set_affinity_mask(5);
    for(;;){
        printf("process id:%d\n",getpid());
    }
    exit(0,"");
}
