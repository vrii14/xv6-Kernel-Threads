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