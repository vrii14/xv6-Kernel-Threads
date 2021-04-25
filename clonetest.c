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
#include "fcntl.h"

int matA[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int matB[3][3] = {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
int matC[3][3];
int matCount = -1;

void matrixThread(void *arg1, void *arg2){
        matCount++;
        int i, j, k;
	for (i = matCount; i < (matCount + 1); i++)
	{
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
			{
				matC[i][j] += matA[i][k] * matB[k][j];
			}
		}
	}
        exit();
}

void matrixMultiplication(){
        printf(1, "Matrix Multiplication test starting\n");
        struct pthread threads[3] ;
        int thread_id[3];
        int arg1 = 10;
        int arg2 = 20;
        int flag = 0;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&threads[i] , &matrixThread, flag, (void *)arg1, (void *)arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        printf(2,"Matrix Multiplication test failed");
                        exit(); 
                }
                sleep(5);
        }
        for(int i=0;i< 3;i++){
                if(thread_join(&threads[i]) != thread_id[i]){
                        printf(2, "Matrix Multiplication test failed\n");
                        exit();
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

void do_something(void *arg1, void *arg2){
        // printf(1, "In do_something ThreadId: %d\n", gettid());
        sleep(2);
        exit();
}

void fileTestFn(void *arg1, void *arg2){
        int fd = *((int *)arg1);
        char ch;
        int n;
        n = read(fd, &ch, 1);
        if(n<0){
                printf(2, "Read error\n");
        }else{
                printf(1, "Inside Thread, ch: %c\n", ch);
        }  
        close(fd);
        sleep(2);
        exit();
}

void fileTest(){
        printf(1, "File test starting\n");
        int fd;
        fd = open("README", O_RDONLY);
       
        if(fd == -1){
                printf(2, "Open failed\n Filetest failed.\n");
                exit();
        }
        char ch;
        int n;
        n = read(fd, &ch, 1);
        if(n<0){
                printf(2, "Read error\n");
        }else{
                printf(1, "Before Thread, ch: %c\n", ch);
        }   
        struct pthread thread;
        int arg1 = fd;
        int arg2 = 20;
        int flag = 0;
        int thread_id = thread_create(&thread, &fileTestFn, flag, (void *)&arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                printf(2,"File test failed");
                exit(); 
        }
        sleep(5);
        if(thread_join(&thread) != thread_id){
                printf(2, "File test failed\n");
                exit();
        }
        n = read(fd, &ch, 1);
        if(n<0){
                printf(2, "Read error\n");
        }else{
                printf(1, "After thread: ch: %c\n", ch);
                printf(1, "File Test passed.\n");
        }     
}

void join_test(void *arg1, void *arg2){
        printf(1, "first: %s,  second: %s\n", arg1, arg2);
        printf(1, "ThreadId: %d\n", gettid());
        exit();
}

void jointest(){
        printf(1, "Join test starting\n");
        struct pthread threads[3] ;
        int thread_id[3];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        for(int i=0; i<3; i++){
                thread_id[i] = thread_create(&threads[i] , &join_test, flag, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        printf(2,"Join test failed");
                        exit(); 
                }
                sleep(5);
        }
        for(int i=2;i>=0;i--){
                if(thread_join(&threads[i]) != thread_id[i]){
                        printf(2, "Join test failed\n");
                        exit();
                }
        }
        printf(1, "Join test passed\n");

}

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

void swaptest(){
        printf(1, "Swap test starting\n");
        struct pthread thread;
        int arg1 = 10;
        int arg2 = 20;
        printf(1,"In swap test before thread, arg1: %d, arg2: %d\n", arg1, arg2);
        int flag = 0;
        int thread_id = thread_create(&thread, &swapTestfn, flag, (void *)&arg1, (void *)&arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                printf(2,"Swap test failed");
                exit(); 
        }
        sleep(5);
        if(thread_join(&thread) != thread_id){
                printf(2, "Swap test failed\n");
                exit();
        }
        printf(1,"In swap test after thread, arg1: %d, arg2: %d\n", arg1, arg2);
        printf(1, "Swap test passed\n");
}

void concurrentFnOne(void *arg1, void*arg2){
        int time = *((int *)arg1);
        sleep(time);
        exit();
}

void concurrentFnTwo(void *arg1, void*arg2){
        int time = *((int *)arg2);
        sleep(time);
        exit();
}

void concurrencyTest(){
        printf(1, "Concurrency Test starting\n");
        struct pthread threads[2];
        int thread_id[2];
        int arg1 = 200;
        int arg2 = 50;
        int flag = 0;
        thread_id[0] = thread_create(&threads[0], &concurrentFnOne, flag, (void *)&arg1, (void *)&arg2);
        thread_id[1] = thread_create(&threads[1], &concurrentFnTwo, flag, (void *)&arg1, (void *)&arg2);

        if ((thread_id[0] == -1) || (thread_id[1] == -1)){
                printf(2,"Clone Failed");
                printf(2,"Concurrency test failed");
                exit(); 
        }
        sleep(5);
        for(int i = 0; i<2; i++){
                if(thread_join(&threads[i]) != thread_id[i]){
                        printf(2, "Concurrency test failed\n");
                        exit();
                }
        }
        printf(1, "Concurrency test passed\n");
}

void stresstestone(){
        printf(1, "Stress test one starting\n");
        struct pthread threads[MAXTHREADS] ;
        int thread_id[MAXTHREADS];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        int num = MAXTHREADS;
        for(int i=0; i<num; i++){
                thread_id[i] = thread_create(&threads[i], &do_something, flag, arg1, arg2);
                if (thread_id[i] == -1){
                        printf(2,"%d\n", thread_id[i]);
                        printf(2,"Clone Failed");
                        printf(2,"Stress test onefailed");
                        exit(); 
                }
                sleep(2);
        }
        int i;
        for(i=0;i<num;i++){
                if(thread_join(&threads[i]) != thread_id[i]){
                        printf(2, "Stress test one failed\n");
                        exit();
                }
        }
        if(i == num){
                printf(1, "Stress test one passed\n");
        }
}

void stresstesttwo(){
        printf(1, "Stress test two starting\n");
        struct pthread threads[100] ;
        int thread_id[100];
        void *arg1 = malloc(1024);
        void *arg2 = malloc(1024);
        strcpy(arg1, "hello");
        strcpy(arg2, "world");
        int flag = 0;
        int count = 0;
        for(int i=0; i<100; i++){
                thread_id[i] = thread_create(&threads[i], &do_something, flag, arg1, arg2);
                if (thread_id[i] == -1){
                        // printf(2,"%d\n", thread_id[i]);
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
                        exit();
                }
        }
        if(i == count){
                printf(1, "Stress test two passed\n");
        }
}

void clone_test(void *arg1, void *arg2){
        int flag = 0;
        struct pthread thread;
        printf(1, "In clone test, ThreadId: %d\n", gettid());
        int thread_id = thread_create(&thread, &do_something, flag, arg1, arg2);
        sleep(2);
        if(thread_join(&thread) != thread_id){
                printf(2, "Nested test failed\n");
                exit();
        }
        printf(1, "After sleep ThreadId: %d\n", gettid());
        exit();
}

void doubleClone(){
        printf(1, "Double Clone test starting\n");
        struct pthread thread;
        int arg1 = 10;
        int arg2 = 20;
        int flag = 0;
        int thread_id = thread_create(&thread, &clone_test, flag, (void *)arg1, (void *)arg2);
        if (thread_id == -1){
                printf(2,"%d\n", thread_id);
                printf(2,"Clone Failed");
                printf(2,"Double Clone test failed");
                exit(); 
        }
        sleep(5);
        if(thread_join(&thread) != thread_id){
                printf(2, "Double Clone test failed\n");
                exit();
        }
        printf(1, "Double Clone test passed\n");
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
        printf(1, "Clone test starting\n");
        
        jointest();
        doubleClone();
        stacktest();
        swaptest();
        concurrencyTest();
        fileTest();
        matrixMultiplication();
        stresstestone();
        stresstesttwo();

        printf(1, "Clone test OK\n");

        exit();
}