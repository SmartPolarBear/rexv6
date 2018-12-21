#include "types.h"
#include "param.h"
#include "defs.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "vesa.h"
#include "terminal.h"

static void bootothers(void);
static void cpu_init(void);


extern "C"
{
   void kernel_setup(void);
}


// Common CPU setup code.
// Bootstrap CPU comes here from kernel_setup().
// Other CPUs jump here from bootother.S.
static inline void cpu_setup(void)
{
   cpu_init();
   scheduler();     // start running processes
}


__attribute__((noreturn)) inline void jmpkstack(void)
{
  char *kstack, *top;
  
  kstack = kalloc();
  if(kstack == 0)
    panic("jmpkstack kalloc");
  top = kstack + PGSIZE;
  asm volatile("movl %0,%%esp; call kernel_setup" : : "r" (top));
  panic("jmpkstack");
}



// Bootstrap processor starts running C code here.
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
int main(void)
{
  init_vesa();
  mpinit();        // collect info about this machine
  lapicinit(mpbcpu());
  seginit();       // set up segments
  kinit();         // initialize memory allocator
  jmpkstack();       // call kernel_setup() on a properly-allocated stack 
}


// Set up hardware and software.
// Runs only on the boostrap processor.
extern "C" void kernel_setup(void)
{
  cprintf("\ncpu%d: starting rexv6\n\n", cpu->id);
  picinit();       // interrupt controller
  ioapicinit();    // another interrupt controller
  consoleinit();   // I/O devices & their interrupts

  uartinit();      // serial port
  kvmalloc();      // initialize the kernel page table
  pinit();         // process table
  tvinit();        // trap vectors
  binit();         // buffer cache
  fileinit();      // file table
  iinit();         // inode cache
  ideinit();       // disk
  
  if(!ismp)
    timerinit();   // uniprocessor timer

  bootothers();    // start other processors
  init_graphic();
  init_terminal();

  // Finish setting up this processor in
  cpu_init();
  sti();           // enable inturrupts
  userinit();      // first user process
  scheduler();     // start running processes
}

// common cpu init code
static void cpu_init(void) 
{
  if(cpunum() != mpbcpu()){
    seginit();
    lapicinit(cpunum());
  }
  vmenable();        // turn on paging
  cprintf("cpu%d: starting\n", cpu->id);
  idtinit();       // load idt register
  xchg(&cpu->booted, 1); // tell bootothers() we're up
}


// Start the non-boot processors.
static void bootothers(void)
{
  extern uchar _binary_bootother_start[], _binary_bootother_size[];
  uchar *code;
  struct cpu *c;
  char *stack;

  // Write bootstrap code to unused memory at 0x7000.
  // The linker has placed the image of bootother.S in
  // _binary_bootother_start.
  code = (uchar*)0x7000;
  memmove(code, _binary_bootother_start, (uint)_binary_bootother_size);

  for(c = cpus; c < cpus+ncpu; c++){
    if(c == cpus+cpunum())  // We've started already.
      continue;

    // Tell bootother.S what stack to use and the address of cpu_setup;
    // it expects to find these two addresses stored just before
    // its first instruction.
    stack = kalloc();
    *(void**)(code-4) = stack + KSTACKSIZE;
    *(void**)(code-8) = (void*)cpu_setup;

    lapicstartap(c->id, (uint)code);

    // Wait for cpu to finish cpu_setup()
    while(c->booted == 0)
      ;
  }
}

// Blank page.

