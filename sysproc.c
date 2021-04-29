#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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
sys_tgkill(void){
  int tgid, tid, sig;

  if(argint(0, &tgid) < 0)
    return -1;
  if(argint(1, &tid) < 0)
    return -1;
  if(argint(2, &sig) < 0)
    return -1;

  return tgkill(tgid, tid, sig);
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
  return myproc()->tgid;
}

int sys_gettid(void)
{
  struct proc *p = myproc();
  if(p->isThread){
    return p->thread_id;
  }
  else{
    return -1;
  }
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_clone(void){
  void (*fcn)(void *, void *);
  void *stack;
  void *arg1;
  void *arg2;
  int flags;
  
  if(argptr(0, (void*)&fcn, sizeof(void*)) < 0)
    return -1;
  if(argptr(1, (void*)&stack, sizeof(void*)) < 0)
    return -1;
  if(argint(2, &flags) < 0)
    return -1;
  if(argptr(3, (void*)&arg1, sizeof(void*)) < 0)
    return -1;
  if(argptr(4, (void*)&arg2, sizeof(void*)) < 0)
    return -1;

  return clone(fcn, stack,flags, arg1, arg2);
}

int sys_join(void){
  int threadId;
  
  if(argint(0,&threadId) < 0)
    return -1;
  
  return join(threadId);
}
