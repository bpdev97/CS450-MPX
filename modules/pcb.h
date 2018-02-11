
#ifndef _PCB_H
#define _PCB_H

typedef struct pcb{
char* name;
int classNum;
int priority; 
int readyState; // readyState == 1 indicates ready
int suspendState; // suspendState == 1 indicates suspended
unsigned char* stackBase;
unsigned char* stackTop;
unsigned char* stack;
int  FreePCB;
struct pcb* nextPcb;
} PCB;

PCB *AllocatePCB();
PCB *SetupPCB(char*, int, int);

PCB *FindPCB(const char *name);
void InsertPCB(PCB *p);
int RemovePCB(PCB *p);


#endif
