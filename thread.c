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

struct thread_table tTable[5];

void threadTableInit(){
  for(int i=0; i< 5; i++){
    tTable[i].present = 0;
    tTable[i].stack = 0;
    tTable[i].threadId = 0;
  }
}
void addToThreadTable(int threadId, void *stack){
  // int i;
  for(int i = 0; i< 5; i++){
    if(tTable[i].present == 0){
      tTable[i].threadId = threadId;
      tTable[i].stack = stack;
      tTable[i].present = 1;
      // printf(1, "%d\n", tTable[0].present);
      break;
    }
  }
}

void removeFromThreadTable(int threadId){
  int count;
  void *stack;
  for(count = 0; count< 5; count++){
    if(tTable[count].present && tTable[count].threadId == threadId){
      stack = tTable[count].stack;
      free(stack);
      tTable[count].present = 0;
      tTable[count].stack = 0;
      tTable[count].threadId = 0;
      break;
    }
  }
}

int thread_create(void (*fcn)(void *, void *), int flags, void *arg1, void *arg2){
  void *stack;
  stack =  malloc(4096);
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
  // if(pid == 0){
  //   //bhjn
  // }
  addToThreadTable(pid, stack);
  return pid;
}

int thread_join(int threadId) {
  int pid = join(threadId);
  if(pid != -1){
    removeFromThreadTable(threadId);
  }else{
    return -1;
  }
  return pid;
}