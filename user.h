#define MAXTHREADS 60
#define CLONE_VM 2
#define CLONE_FILES 4
#define CLONE_FS 8
#define CLONE_PARENT 16
#define CLONE_THREAD 32
struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int clone(void(*)(void *, void *), void *, int, void *, void *);
int join(int);
int gettid(void);
int tgkill(int, int, int);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
struct pthread
{
    uint threadId;
    void *stack;
};
int thread_create(struct pthread *, void (*)(void *, void *), int, void *, void *);
int thread_join(struct pthread *);

struct t_lock{
    uint ticket;
};

void tlock_init(struct t_lock *);
void tlock_acquire( struct t_lock *);
void tlock_release(struct t_lock *);
