struct thread_table {
    uint present;
    uint threadId;
    void *stack;
};

void threadTableInit();
int thread_create(void (*)(void *, void *), int, void *, void *);
int thread_join(int);