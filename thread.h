struct pthread
{
    uint threadId;
    void *stack;
};

int thread_create(struct pthread *, void (*)(void *, void *), int, void *, void *);
int thread_join(struct pthread *);