#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

static inline uint
xaddl(volatile uint *addr, uint value)
{
  // reference from: https://en.wikipedia.org/wiki/Fetch-and-add

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xaddl %0, %1" :
               "+r" (value), "+m" (*addr) :
                : // no input-only
               "memory");
  return value;
}

void
tlock_init(struct t_lock *lk)
{
  lk->ticket = 0;
}

void 
tlock_acquire(struct t_lock *lk)
{
while(xaddl(&lk->ticket, 1) != 0) {
        ;
    }
  __sync_synchronize();

}

void
tlock_release(struct t_lock *lk)
{
  __sync_synchronize();
  lk->ticket = 0;
}
int thread_create(struct pthread *thread, void (*fcn)(void *, void *), int flags, void *arg1, void *arg2){
  thread->stack =  malloc(4096);
  if(!thread->stack) {
    printf(2,"Malloc Failed");
    return -1;
  }
  thread->threadId = clone( (void *) fcn, thread->stack, flags, arg1, arg2);

  if (thread->threadId < 0){
    // printf(2, "pid: %d\n", pid);
    printf(2,"Clone Failed");
    return -1;
  }
  return thread->threadId;
}

int thread_join(struct pthread *thread) {
  int pid = join(thread->threadId);
  if((pid != -1) && (pid == thread->threadId)){
    free(thread->stack);
  }else{
    free(thread->stack);
    return -1;
  }
  return pid;
}
