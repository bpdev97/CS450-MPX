
#ifndef _PCB_H
#define _PCB_H

typedef struct pcb{
char* name;
int classNum;
int priority; 
// readyState == 1 indicates ready
//            == 0 running
//            == -1 blocked
int readyState; 
int suspendState; // suspendState == 1 indicates suspended
unsigned char* stackBase;
unsigned char* stackTop;
struct context* context;
struct pcb* nextPcb;
} PCB;

typedef struct context{
  u32int gs, fs, es, ds; 
  u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32int eip, cs, eflags;
} CONTEXT;


PCB *AllocatePCB();
PCB *SetupPCB(char*, int, int);

PCB *FindPCB(const char *name);
void InsertPCB(PCB *p);
int RemovePCB(PCB *p);
int FreePCB(PCB* freepcb);

u32int* sys_call(CONTEXT* registers);
void IdleProcess(void);

#endif
