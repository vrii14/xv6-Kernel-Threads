int thread_create(void (*)(void *, void *), int, void *, void *);
int thread_join(int);