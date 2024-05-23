#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("memory using:%d\n", memsize());
  printf("adding 20000 bytes of memory\n");
  char* c=malloc(20000);
  printf("memory using:%d\n", memsize());
  printf("cleaning added bytes\n");
  free(c);
  printf("memory using:%d\n", memsize());

  exit(0,"");
}
