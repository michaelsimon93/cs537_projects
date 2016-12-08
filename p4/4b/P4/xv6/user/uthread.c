#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

void
lock_init(lock_t *l)
{
	//up for grabs
	l->flag = 0;
}

void
lock_acquire(lock_t *l)
{
	//spin-wait
	while (xchg(&l->flag, 1) == 1) ;
}

void
lock_release(lock_t *l)
{
	//back up for grabs
	l->flag = 0;
}

int
thread_create(void (*start_routine)(void*), void *arg) {
	//allocated new stack;
	int off = (int)sbrk(0);
	void *sp = malloc(0x1000);
	if((uint)sp%4096 != 0) sp -= (uint)sp%4096;	
	if (off%4096 != 0) sbrk(4096-off%4096);

	return clone(start_routine, arg, sp);
}

int
thread_join() {
	void *sp;
	int retAdd = join(&sp);
	free(sp);
	return retAdd;
}
