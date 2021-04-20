#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "thread.h"


int thread_create(void (*fcn)(void *, void *), int flags, void *arg1, void *arg2){
  void *stack;
  stack =  malloc(PGSIZE*2);
  if(!stack) {
    printf(2,"Malloc Failed");
    return -1;
  }
  int pid = clone( (void *) fcn, stack, flags, arg1, arg2);

  if (pid < 0){
    // printf(2, "pid: %d\n", pid);
    printf(2,"Clone Failed");
    return -1;
  }
  return pid;
}

int thread_join(int threadId) {
    int pid = join(threadId);
    return pid;
}