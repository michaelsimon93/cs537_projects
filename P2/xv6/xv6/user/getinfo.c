#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "../kernel/pstat.h"

int
main(int argc, char *argv[])
{
	

	
  struct pstat *stat = malloc(sizeof(struct pstat));
  getpinfo(stat);
  int x;
  for (x = 0; x < NPROC; x++) {
	  printf(1, "%d\t", stat->pid[x]);
	  printf(1, "%d\t", stat->priority[x]);	  
	  printf(1, "%d\t", stat->startTick[x]);	  
	  printf(1, "%d\t", stat->endTick[x]);
	  printf(1, "%d\t", stat->totalTicks[x]);
	  printf(1, "%d\t", stat->ticks[x][0]);
	  printf(1, "%d\t", stat->ticks[x][1]);
	  printf(1, "%d\t", stat->ticks[x][2]);
	  printf(1, "%d\t", stat->ticks[x][3]);
	  printf(1,"\n");
	  
  }
  exit();
}


