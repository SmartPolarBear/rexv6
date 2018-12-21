#pragma once
#include <types.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct stat;

#ifdef __cplusplus
}
#endif // __cplusplus

// bio.c

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
#ifdef __cplusplus
}
#endif // __cplusplus


// console.c
#include <console.h>

// exec.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             exec(char*, char**);

#ifdef __cplusplus
}
#endif // __cplusplus


// file.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, char*, int n);
int             filestat(struct file*, struct stat*);
int             filewrite(struct file*, char*, int n);
#ifdef __cplusplus
}
#endif // __cplusplus

// fs.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
void            iinit(void);
void            ilock(struct inode*);
void            iput(struct inode*);
void            iunlock(struct inode*);
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, char*, uint, uint);
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, char*, uint, uint);
#ifdef __cplusplus
}
#endif // __cplusplus

// ide.c

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            ideinit(void);
void            ideintr(void);
void            iderw(struct buf*);
#ifdef __cplusplus
}
#endif // __cplusplus

// ioapic.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            ioapicenable(int irq, int cpu);
extern uchar    ioapicid;
void            ioapicinit(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// kalloc.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

char*           kalloc(void);
void            kfree(char*);
void            kinit(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// kbd.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            kbdintr(void);
void			kbdinit(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// lapic.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             cpunum(void);
extern volatile uint*    lapic;
void            lapiceoi(void);
void            lapicinit(int);
void            lapicstartap(uchar, uint);
void            microdelay(int);
#ifdef __cplusplus
}
#endif // __cplusplus

// mp.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int      ismp;
int             mpbcpu(void);
void            mpinit(void);
void            mpstartthem(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// picirq.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            picenable(int);
void            picinit(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// pipe.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             pipealloc(struct file**, struct file**);
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, char*, int);
int             pipewrite(struct pipe*, char*, int);
#ifdef __cplusplus
}
#endif // __cplusplus

// proc.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct proc*    copyproc(struct proc*);
void            exit(void);
int             fork(void);
int             growproc(int);
int             kill(int);
void            pinit(void);
void            procdump(void);
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
void            sleep(void*, struct spinlock*);
void            userinit(void);
int             wait(void);
void            wakeup(void*);
void            yield(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// swtch.S
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
void            swtch(struct context**, struct context*);
#ifdef __cplusplus
}
#endif // __cplusplus


// spinlock.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            acquire(struct spinlock*);
void            getcallerpcs(void*, uint*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            pushcli(void);
void            popcli(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// string.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*			memcpy(void *dst, const void *src, uint n);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);
#ifdef __cplusplus
}
#endif // __cplusplus

// syscall.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int             argint(int, int*);
int             argptr(int, char**, int);
int             argstr(int, char**);
int             fetchint(struct proc*, uint, int*);
int             fetchstr(struct proc*, uint, char**);
void            syscall(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// timer.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
void            timerinit(void);
#ifdef __cplusplus
}
#endif // __cplusplus

// trap.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            idtinit(void);
extern uint     ticks;
void            tvinit(void);
extern struct spinlock tickslock;
#ifdef __cplusplus
}
#endif // __cplusplus

// uart.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            uartinit(void);
void            uartintr(void);
void            uartputc(int);
#ifdef __cplusplus
}
#endif // __cplusplus

// vm.c
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void            seginit(void);
void            kvmalloc(void);
void            vmenable(void);
pde_t*          setupkvm(void);
char*           uva2ka(pde_t*, char*);
int             allocuvm(pde_t*, uint, uint);
int             deallocuvm(pde_t*, uint, uint);
void            freevm(pde_t*);
void            inituvm(pde_t*, char*, uint);
int             loaduvm(pde_t*, char*, struct inode*, uint, uint);
pde_t*          copyuvm(pde_t*, uint);
void            switchuvm(struct proc*);
void            switchkvm(void);
int             copyout(pde_t*, uint, void*, uint);
#ifdef __cplusplus
}
#endif // __cplusplus

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

//#endif // _DEFS_H_
