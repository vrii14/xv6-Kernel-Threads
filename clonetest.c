#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

void do_something(void *arg1, void*arg2){
        printf(1, "Child pid : %d %c\n", arg1, arg2);
        exit();
}
int main() {
        void *stack = malloc(KSTACKSIZE);    // Stack for new process
        //void *arg1 = malloc(1024);
        //void *arg2 = malloc(1024);
        int flag = 0;
        int arg1 = 10;
        char arg2 = 'a';
        printf(1, "Malloc done\n");
        if(!stack) {
                printf(2,"Malloc Failed");
                exit();
        }
        // if(clone( &do_something, stack, flag, arg1, arg2) < 0 ){
        //         printf(2,"Clone Failed");
        //         exit();
        // }
        if(!clone( &do_something, stack, flag, (void *)arg1, (void *)&arg2)){
                printf(1, "try\n");
        }
        wait();
        // printf(1, "pid: %d\n", pid);
        printf(1,"Parent pid : 10\n");
        sleep(1);       // Add sleep so we can she both processes output
        free(stack);
        exit();
}