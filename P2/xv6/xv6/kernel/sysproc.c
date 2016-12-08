#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



int
sys_getpinfo(void)
{
	
	
	struct pstat **pst = (struct pstat **)kalloc();
	*pst = *(struct pstat**)(proc->tf->esp + 4);
	struct proc *table = proc->table;

	int x;
	for (x = 0; x < NPROC; x++) {
		if (table[x].state == UNUSED && table[x].endTick != 0) {
			 (*(pst))->inuse[x] = 0;
			 (*(pst))->pid[x] = table[x].pid;
			 (*(pst))->priority[x] = table[x].priInfo;
			 (*(pst))->startTick[x] = table[x].startTick;
			 (*(pst))->endTick[x] = table[x].endTick;
			 (*(pst))->totalTicks[x] = table[x].totalTicks;
			 int y;
			 for (y = 0; y < 4; y++) (*(pst))->ticks[x][y] = NULL;
		}
		else {
			(*(pst))->inuse[x] = 1;
			(*(pst))->pid[x] = table[x].pid;
			 (*(pst))->priority[x] = table[x].priority;
			 (*(pst))->startTick[x] = table[x].startTick;
			 (*(pst))->endTick[x] = table[x].endTick;
			 (*(pst))->totalTicks[x] = table[x].totalTicks;
			int y;
			for (y = 0; y < 4; y++) (*(pst))->ticks[x][y] = table[x].numTicks[y];
		}
	}
	return 0;
}
