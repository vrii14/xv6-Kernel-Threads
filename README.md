# xv6-Kernel-Threads

  This project is an implementation of Kernel Threads in xv6 Operating System.  
  It is built based on the guidelines of [this](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-xv6-threads) repository.
  Userland threading library is also added with one-to-one mapping and synchronization with ticket-locks.  
  Appropriate system calls added for the implementation of threads and modified current system calls.  
  
 
 ## Team Members: 
    1. Vrinda Ahuja
    2. Mrunal Kotkar
    
 ## Added System Calls:
 
 ### CLONE:
 
    int clone(void(*fcn)(void *, void *), void *stack, int flags, void *arg1, void *arg2);
 * Clone system call to create a thread of a process and is called from wrapper function of userland threading library
 * _fcn_ - Function pointer which runs in the created thread.
 * _flags_ - Flags for the clone system call.
 * _stack_ - Child stack to pass the function(fcn) arguments.
 * _arg1_ & _arg2_ - Arguments to function fcn.

### JOIN:

    int join(int threadId);  
  * Join system call to wait for execution of current thread and block the other threads and finally free thread resources after execution.
  * _threadId_ - thread Id of the executing thread.

### GETTID:

    int gettid();
* gettid system call to get the thread Id of currently executing thread
    
### TGKILL:

    int tgkill(int tgid, int tid, int sig);
* tgkill system call to kill the thread from thread group with thread group id as tgid and thread id as tid.
* _tgid_ - thread group id (parent id)
* _tid_ - thread id to be killed
* _sig_ - signal number to be sent

## Modified System Calls:

### FORK:

 * Setting up tgid for the process.
 * Setting some other fields added to proc structure for implementation of threads.

### EXIT:

  * Changing one condition in closing all file descriptors if it is a thread and CLONE_FILE flag is set.

### KILL:

  * Adding condition for killing a parent process if it has child threads, kill all threads with tgkill.


## Userland Threading Library:

  Functions implemented for userland threading library are: 
* _thread_create_ - Malloc the stack for thread and then calls clone system call.
* _thread_join_ - Free the stack for the thread and call join system call.
* _tlock_acquire_ - Acquires a ticket lock
* _tlock_release_ - Reelease the ticket lock
* _tlock_init_ - Initialize the ticket lock

## Testing Suite:

  Added a testing code to test all the functionality of Kernel threads with different test cases.
