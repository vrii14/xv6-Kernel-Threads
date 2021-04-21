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

void do_something(void *arg1, void *arg2){
        printf(1, "In do_something ThreadId: %d\n", gettid());
        exit();
}
void join_test(void *arg1, void *arg2){
        // printf(1, "Child pid : %d\n", pid);
        printf(1, "first: %s\n", arg1);
        printf(1, "second: %s\n", arg2);
        printf(1, "ThreadId: %d\n", gettid());
        exit();
}
void clone_test(void *arg1, void *arg2){
        int flag = 0;
        printf(1, "In clone test, ThreadId: %d\n", gettid());
        int thread_id = thread_create(&do_something, flag, arg1, arg2);
        sleep(2);
        join(thread_id);
        printf(1, "After sleep ThreadId: %d\n", gettid());
        exit();
}
void stresstest(){
        printf(1, "Stress test starting\n");
        int thread_id[MAXTHREADS];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        int num = MAXTHREADS;
        for(int i=0; i<num; i++){
                thread_id[i] = thread_create(&do_something, flag, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        printf(2,"Stress test failed");
                        exit(); 
                }
                sleep(2);
        }
        int i;
        for(i=0;i<num;i++){
                thread_join(thread_id[i]);
        }
        if(i == num){
                printf(1, "Stress test passed\n");
        }
}
void jointest(){
        printf(1, "Join test starting\n");
        int thread_id[3];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&join_test, flag, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        printf(2,"Join test failed");
                        exit(); 
                }
                sleep(5);
        }
        for(int i=2;i>=0;i--){
                thread_join(thread_id[i]);
        }
        printf(1, "Join test passed\n");

}

void doubleClone(){
        printf(1, "Nested test starting\n");
        int arg1 = 10;
        int arg2 = 20;
        int flag = 0;
        int thread_id = thread_create(&clone_test, flag, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                printf(2,"Nested test failed");
                exit(); 
        }
        sleep(5);
        thread_join(thread_id);
        printf(1, "Nested test passed\n");
}

void stacktest(){
        printf(1, "Stack test starting\n");
        int arg1 = 10;
        int arg2 = 20;
        int flag = 0;
        void *stack;
        stack =  malloc(4096);
        if(!stack) {
                printf(2,"Malloc Failed");
                printf(2,"Stack test failed");
                exit();
        }
        int thread_id = clone(&do_something, stack, flag, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                printf(2,"Stack test failed");
                exit(); 
        }
        sleep(2);
        join(thread_id);
        printf(1, "Stack test passed\n");
}

int main(int argc, char *argv[]) {
        threadTableInit();
        
        printf(1, "Clone test starting\n");
        
        jointest();
        doubleClone();
        stacktest();
        stresstest();

        printf(1, "Clone test OK\n");

        exit();
}