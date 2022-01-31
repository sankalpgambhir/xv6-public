#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// print hello to the screen
int
sys_hello(void)
{
  cprintf("Hello\n");
  return 0;
}

// prints an input string to the screen
int
sys_helloYou(void)
{
  char* inp = (char*)0;

  if(argstr(0, &inp) < 0){
    return -1;
  }

  cprintf(inp);
  cprintf("\n");
  return 0;
}

// prints number of active processes right now
int 
sys_getNumProc(void)
{
  return activeProc();
}

// prints the ancestry of a process
int 
sys_showAncestry(void)
{
  struct proc * pr = myproc();

  while(!isInit(pr->pid)){
    pr = pr->parent;
    cprintf("%d\n", pr->pid);
  }

  return 0;
}