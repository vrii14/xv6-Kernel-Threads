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
        // int pid=getpid();
        // printf(1, "Child pid : %d\n", pid);
        // printf(1, "first: %s\n", arg1);
        // printf(1, "second: %s\n", arg2);
        printf(1, "%d\n", gettid());
        exit();
}

void do_anything(void *arg1, void *arg2){
        printf(1, "Helloooo\n");
        exit();
}


int main() {

        printf(1, "Clone test\n");
        //int child_pid;
        int thread_id[3];
        //int pid;
        // void *stack = malloc(KSTACKSIZE);    // Stack for new process
        // void *stack2 = malloc(KSTACKSIZE);
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        void *stack;

        
        
        for(int i=0; i<3; i++){
                stack = malloc(5000);

                if(!stack) {
                        printf(2,"Malloc Failed");
                        exit();
                }

                thread_id[i] = clone( &do_something, stack, flag, arg1, arg2);
                if (thread_id[i] < 0){
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

        
        // sleep(5);

        // child_pid2 = clone( &do_anything, stack2, flag, arg1, arg2);
        // if (child_pid2 < 0){
        //         printf(2,"Clone Failed");
        //         exit(); 
        // }
        
        // sleep(5);

        // printf(1, "%d\n", child_pid1);

        // join(child_pid1);

        // sleep(5);
        
        // printf(1, "%d\n", child_pid2);

        
        // join(child_pid2);

        // printf(1, "Clone test OK\n");
        // 
        
        exit();
}