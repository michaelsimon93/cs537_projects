#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "x86.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}

//not sure where else this should go, but oh well
//functions need malloc and free
/*
void
lock_init(lock_t* l)
{
	//up for grabs
	l->flag = 0;
}

void
lock_aquire(lock_t *l)
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
	if (off%4096 != 0){
		sbrk(4096-off%4096);
	}
	void *sp = malloc(0x1000);
	if((uint)sp%4096 != 0) {
		sp -= (uint)sp%4096;
	}

	return clone(start_routine, arg, sp);
}

int
thread_join() {
	void *sp;
	int retAdd = join(&sp);
	free(sp);
	return retAdd;
}
*/
