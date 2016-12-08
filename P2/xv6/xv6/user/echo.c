#include "types.h"
#include "stat.h"
#include "user.h"
#include "../kernel/pstat.h"

/*int
main(int argc, char *argv[])
{
  struct pstat *stat = malloc(sizeof(struct pstat));
  getpinfo(stat);
  printf(1,"~~~~~~~~~~\n");
  int x;
  for (x = 0; x < NPROC; x++) {
	  printf(1,"%d\t",stat->inuse[x]);
  }
  printf(1,"\n");
  exit();
}*/


#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++)
    printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
  exit();
}
