#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

void do_something(void *arg1, void *arg2){
        // printf(1, "Child pid : %d\n", pid);
        // printf(1, "first: %s\n", arg1);
        // printf(1, "second: %s\n", arg2);
        printf(1, "ThreadId: %d\n", gettid());
        exit();
}


int main() {

        printf(1, "Clone test\n");
        int thread_id[3];
        // void *stack = malloc(KSTACKSIZE);    // Stack for new process
        // void *stack2 = malloc(KSTACKSIZE);
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        void *stack;

        for(int i=0; i<3; i++){
                stack = malloc(KSTACKSIZE*2);

                if(!stack) {
                        printf(2,"Malloc Failed");
                        exit();
                }

                thread_id[i] = clone( &do_something, stack, flag, arg1, arg2);
                if (thread_id[i] < 0){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        exit(); 
                }
                
                sleep(2);
                free(stack);
        }

        for(int i=0;i<3;i++){
                join(thread_id[i]);
        }
        
        // child_pid1 = clone( &do_something, stack, flag, arg1, arg2);
        // if (child_pid1 < 0){
        //         printf(2,"Clone Failed");
        //         exit(); 
        // }

        printf(1, "Clone test OK\n");

        exit();
}