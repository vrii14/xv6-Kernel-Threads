#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "fcntl.h"

struct t_lock lock;
void tinit(){
        tlock_init(&lock);
}
int c =0 , c1 = 0, c2 = 0, run = 1;
int fd2;
int flag = 0;
int matA[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int matB[3][3] = {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
int matC[3][3];

//thread for matrix multiplication
void matrixThread(void *arg1, void *arg2){
        int *num = (int *)arg2;
        int start = 0, end = 0;
        int i, j, k;
        if(*num == 0){
		start = 0;
		end = 1;
	}
	if(*num == 1){
		start = 1;
		end = 2;
	}
	if(*num == 2){
		start = 2;
		end = 3;
	}
	for(i = start; i < end; i++) {
		for(j = 0; j < 3; j++) {
			matC[i][j] = 0;
			for(k = 0; k < 3; k++) {
				matC[i][j] += matA[i][k] * matB[k][j];
			}
		}
	}
        exit();
}

// three threads to simultaneously perform matrix multiplication
void matrixMultiplication(){
        printf(1, "Matrix Multiplication test starting\n");
        struct pthread threads[3] ;
        int thread_id[3];
        int arg1 = 10;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&threads[i] , &matrixThread, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)&i);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        flag = 1;
                        printf(2,"Clone Failed");
                        printf(2,"Matrix Multiplication test failed");
                }
                sleep(5);
        }
        if(flag == 0){
                for(int i=0;i< 3;i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Matrix Multiplication test failed\n");
                        }
                }
                for (int i = 0; i < 3; i++)
                {
                        for (int j = 0; j < 3; j++)
                        {
                                printf(1, "%d  ", matC[i][j]);
                        }
                        printf(1, "\n");
                }
                printf(1, "Matrix Multiplication test passed\n");
        }
}
//demo thread
void do_something(void *arg1, void *arg2){
        sleep(2);
        exit();
}
//join test thread
void join_test(void *arg1, void *arg2){
        printf(1, "first: %s,  second: %s\n", arg1, arg2);
        printf(1, "ThreadId: %d\n", gettid());
        exit();
}
//confirmance test for join system call 
//reverse joining of threads
void jointest(){
        printf(1, "Join test starting\n");
        struct pthread threads[3] ;
        int thread_id[3];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        // int flag = 0;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&threads[i] , &join_test, CLONE_VM | CLONE_THREAD, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        flag = 1;
                        printf(2,"Join test failed");
                }
                sleep(5);
        }
        if(flag == 0){
                for(int i=2;i>=0;i--){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Join test failed\n");
                        }
                }
                printf(1, "Join test passed\n");

        }
}
//swap test thread
void swapTestfn(void *a, void*b){
        int *x = (int *)a;
        int *y  = (int *)b;
        int temp;
        temp = *x;
        *x = *y;
        *y = temp;
        printf(1, "In swaptestfn: %d %d\n", *x, *y);
        exit();
}
// confirmance test for swapping of two integers using clone
void swaptest(){
        printf(1, "Swap test starting\n");
        struct pthread thread;
        int arg1 = 10;
        int arg2 = 20;
        printf(1,"In swap test before thread, arg1: %d, arg2: %d\n", arg1, arg2);
        int thread_id = thread_create(&thread, &swapTestfn, CLONE_VM | CLONE_THREAD, (void *)&arg1, (void *)&arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Swap test failed");
        }
        if(flag == 0){
                sleep(5);
                if(thread_join(&thread) != thread_id){
                        printf(2, "Swap test failed\n");
                }
                printf(1,"In swap test after thread, arg1: %d, arg2: %d\n", arg1, arg2);
                printf(1, "Swap test passed\n");
        }
}
//sleeping amount 200msec
void concurrentFnOne(void *arg1, void*arg2){
        int time = *((int *)arg1);
        sleep(time);
        exit();
}
//sleeping amount 50msec
void concurrentFnTwo(void *arg1, void*arg2){
        int time = *((int *)arg2);
        sleep(time);
        exit();
}
// concurrency test for two threads sleeping different 
//amount of time and joining correctly
void concurrencyTest(){
        printf(1, "Concurrency Test starting\n");
        struct pthread threads[2];
        int thread_id[2];
        int arg1 = 200;
        int arg2 = 50;
        thread_id[0] = thread_create(&threads[0], &concurrentFnOne, CLONE_VM | CLONE_THREAD, (void *)&arg1, (void *)&arg2);
        thread_id[1] = thread_create(&threads[1], &concurrentFnTwo, CLONE_VM | CLONE_THREAD, (void *)&arg1, (void *)&arg2);

        if ((thread_id[0] == -1) || (thread_id[1] == -1)){
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Concurrency test failed");
        }
        if(flag == 0){
                sleep(5);
                for(int i = 0; i<2; i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Concurrency test failed\n");
                        }
                }
                printf(1, "Concurrency test passed\n");
        }
}
//synchroniztion thread one         
void syncFnOne(void *arg1, void*arg2){
        while(run == 1){
                tlock_acquire(&lock);
                c++;
                tlock_release(&lock);
                c1++;
        }
        exit();
}

//synchroniztion thread two         
void syncFnTwo(void *arg1, void*arg2){
        while(run == 1){
                tlock_acquire(&lock);
                c++;
                tlock_release(&lock);
                c2++;
        }
        exit();
}
// confirmance test for synchronization using ticket lock
void syncTest(){
        printf(1, "Synch Test starting\n");
        struct pthread threads[2];
        int thread_id[2];
        int arg1 = 200;
        int arg2 = 50;
        thread_id[0] = thread_create(&threads[0], &syncFnOne, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        thread_id[1] = thread_create(&threads[1], &syncFnTwo, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);

        if ((thread_id[0] == -1) || (thread_id[1] == -1)){
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Synch test failed");
        }
        if(flag == 0){
                sleep(2);
                run = 0;
                for(int i = 0; i<2; i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Synch test failed\n");
                        }
                }
                printf(1, "c: %d c1+c2: %d c1: %d c2: %d\n", c, c1+c2, c1, c2);
                printf(1, "Synch test passed\n");
        }
}
// lock test thread 
void lockfn(void *arg1, void *arg2){
        tlock_acquire(&lock);
        printf(1, "tid: %d\n", gettid());
        tlock_release(&lock);
        exit();
}
// confirmace test for gettid system call and 
// correct synchronization using ticket lock
void locktest(){
      printf(1, "Lock Test starting\n");
        struct pthread threads[2];
        int thread_id[2];
        int arg1 = 200;
        int arg2 = 200;
        tlock_init(&lock);
        thread_id[0] = thread_create(&threads[0], &lockfn, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        thread_id[1] = thread_create(&threads[1], &lockfn, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);

        if ((thread_id[0] == -1) || (thread_id[1] == -1)){
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Lock test failed");
        }
        if(flag == 0){
                for(int i = 0; i<2; i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Lock test failed\n");
                        }
                }        
                printf(1, "Lock test passed\n"); 
        }
}
void locktwofn(void *arg1, void *arg2){
        printf(1, "tid: %d\n", gettid());
        exit();
}
//same test without ticket lock
void locktwotest(){
      printf(1, "Lock Test Two starting\n");
        struct pthread threads[3];
        int thread_id[3];
        int arg1 = 200;
        int arg2 = 200;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&threads[i] , &locktwofn, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        flag = 1;
                        printf(2,"Lock test two failed");
                }
        }
        if(flag == 0){
                for(int i = 0; i<2; i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Lock test two failed\n");
                        }
                }        
                printf(1, "Lock test Two passed\n");  
        }
}
//Stress Test One creating maximum threads without exhaustion of kernel
void stresstestone(){
        printf(1, "Stress test one starting\n");
        struct pthread threads[MAXTHREADS] ;
        int thread_id[MAXTHREADS];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int num = MAXTHREADS;
        for(int i=0; i<num; i++){
                thread_id[i] = thread_create(&threads[i], &do_something, CLONE_VM | CLONE_THREAD, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        flag = 1;
                        printf(2,"Stress test onefailed");
                }
                sleep(2);
        }
        if(flag == 0){
                int i;
                for(i=0;i<num;i++){
                        if(thread_join(&threads[i]) != thread_id[i]){
                                printf(2, "Stress test one failed\n");
                        }
                }
                if(i == num){
                        printf(1, "Stress test one passed\n");
                }
        }
}
//stress test two with kernel exhaustion
void stresstesttwo(){
        printf(1, "Stress test two starting\n");
        struct pthread threads[100] ;
        int thread_id[100];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int count = 0;
        for(int i=0; i<100; i++){
                thread_id[i] = thread_create(&threads[i], &do_something, CLONE_VM | CLONE_THREAD, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"Clone Failed\n");
                        break;
                }
                count++;
                sleep(2);
        }
        int i;
        for(i=0;i<count;i++){
                if(thread_join(&threads[i]) != thread_id[i]){
                        printf(2, "Stress test two failed\n");
                }
        }
        if(i == count){
                printf(1, "Stress test two passed\n");
        }
        
}
//thread for forktest which calls fork
void fork_test(void *arg1, void *arg2){
        printf(1, "In fork test\n");
        int pid = fork();
        if(pid<0){
                printf(2,"Fork Failed");
                printf(2,"Fork Clone test failed");
        }
        if(pid == 0){
                // printf(1, "In child process, pid: %d\n", pid);
                // sleep(5);
                char *execargs[] = {"ls", 0};
                exec("ls", execargs);
        }else{
                // printf(1, "In parent process, pid: %d\n", pid);
                wait();
        }
        exit();
}
// test to check correct calling of fork inside a thread
void forkInClone(){
        printf(1, "Fork Clone test starting\n");
        struct pthread thread;
        int arg1 = 10;
        int arg2 = 20;
        int thread_id = thread_create(&thread, &fork_test, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Fork Clone test failed");
        }
        if(flag == 0){
                sleep(5);
                if(thread_join(&thread) != thread_id){
                        printf(2, "Fork Clone test failed\n");
                }
                printf(1, "Fork Clone test passed\n");
        }
}
// double clone thread function
void clone_test(void *arg1, void *arg2){
        struct pthread thread;
        printf(1, "In clone test, ThreadId: %d\n", gettid());
        int thread_id = thread_create(&thread, &do_something, CLONE_VM | CLONE_THREAD, arg1, arg2);
        sleep(2);
        if(thread_join(&thread) != thread_id){
                printf(2, "Nested test failed\n");
                exit();
        }
        printf(1, "After sleep ThreadId: %d\n", gettid());
        exit();
}
// test to check clone inside clone 
void doubleClone(){
        printf(1, "Double Clone test starting\n");
        struct pthread thread;
        int arg1 = 10;
        int arg2 = 20;
        int thread_id = thread_create(&thread, &clone_test, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Double Clone test failed");
        }
        if(flag == 0){
                sleep(5);
                if(thread_join(&thread) != thread_id){
                        printf(2, "Double Clone test failed\n");
                }
                printf(1, "Double Clone test passed\n");
        }
}
// stack test to malloc memory which is not a multiple of pgsize
void stacktest(){
        printf(1, "Stack test starting\n");
        int arg1 = 10;
        int arg2 = 20;
        void *stack;
        stack =  malloc(2000);
        if(!stack) {
                printf(2,"Malloc Failed");
                printf(2,"Stack test failed");
        }
        int thread_id = clone(&do_something, stack, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Stack test failed");
        }
        if(flag == 0){
                sleep(2);
                join(thread_id);
                printf(1, "Stack test passed\n");
        }
}
// tgkill test function
void func_tgkill(void *arg1, void *arg2){
        printf(1, "Before calling tgkill\n");
        int tgid = getpid();
        int tid = gettid();
        if(tgkill(tgid, tid, 0) == -1)
                printf(2, "tgkill test failed\n");

        printf(1, "After calling tgkill\n");
        exit();
}

//confirmance test for tgkill system call
void tgkilltest(){
        printf(1, "tgkill test starting\n");

        int arg1 = 10;
        int arg2 = 20;
        void *stack;
        stack =  malloc(4096);
        if(!stack) {
                printf(2,"Malloc Failed");
        }
        int thread_id = clone(&func_tgkill, stack,CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"Clone Failed");
                flag = 1;
        }
        if(flag == 0){
                sleep(5);       
                printf(1, "tgkill test passed\n");
        }
       
}

void files_func(void *arg1, void *arg2)
{
	close(fd2);
        exit();
}

void filesflagtest(){
        printf(1, "Files Flag test starting\n");

        char *stack = malloc(4096);
	char buf[256];
        int arg2 = 20;
        int status;

	fd2 = open("README", O_RDWR);
	if (fd2 == -1) {
                printf(2,"Open Failed");
		exit();
	}
	if (!stack) {
                printf(2,"Malloc Failed");
		exit();
	}

        int pid = clone(&files_func, stack, CLONE_VM | CLONE_FILES | CLONE_THREAD, (void *)buf, (void *)arg2);
	if ( pid== -1) {
                printf(2,"Clone Failed");
		exit();
	}

        join(pid);

	status = read(fd2, buf, 5);
	if (status < 0) {
                printf(2,"Read Failed");
                printf(1, "Files Flag test passed\n");
		exit();
	}
	printf(1, "Parent read:%s\n", buf);
	close(fd2);

        printf(1, "Files Flag test passed\n");

}

void parent_fn(void *arg1, void *arg2)
{
        exit();
}
//testing working of join if parent flag 
void parentFlagtest(){
        printf(1, "Parent Flag Test starting\n");
        int arg1 = 10;
        int arg2 = 20;
        void *stack;
        stack =  malloc(4096);
        if(!stack) {
                printf(2,"Malloc Failed");
                printf(2,"Parent Flag test failed");
        }
        int thread_id = clone(&parent_fn, stack, CLONE_VM | CLONE_PARENT | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Parent Flag test failed");
        }
        if(flag == 0){
                sleep(1);
                join(thread_id);
                printf(1, "Parent Flag test passed\n");
        }
}

void thread_fcn(void *arg1, void *arg2)
{
        int pid = getpid();
        printf(1, "thread group id : %d\n", pid);
        exit();
}
//test to check working if thread flag is set
void threadFlagTest(){
        printf(1, "Thread Flag Test starting\n");
        int arg1 = 10;
        int arg2 = 20;
        void *stack;
        stack =  malloc(4096);
        if(!stack) {
                printf(2,"Malloc Failed");
                printf(2,"Thread Flag test failed");
        }
        int thread_id = clone(&thread_fcn, stack, CLONE_VM | CLONE_THREAD, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                flag = 1;
                printf(2,"Thread Flag test failed");
        }
        if(flag == 0){
                sleep(1);
                join(thread_id);
                printf(1, "Thread Flag test passed\n");       
        }
}

int main(int argc, char *argv[]) {
        printf(1, "Clone test starting\n");
        
        tinit();
        syncTest();
        threadFlagTest();
        jointest();
        doubleClone();
        stacktest();
        swaptest();
        concurrencyTest();
        locktwotest();
        matrixMultiplication();
        forkInClone();
        stresstestone();
        stresstesttwo();
        locktest();
        tgkilltest();
        // filesflagtest();
        parentFlagtest();

        printf(1, "Clone test OK\n");

        exit();
}
