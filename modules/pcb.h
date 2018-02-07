
#ifndef _PCB_H
#define _PCB_H

typedef struct{
char* name;
int classNum;
int priority; 
int readyState;
int suspendState;
unsigned char* stackBase;
unsigned char* stackTop;
struct pcb* nextPcb;
}pcb;

#endif
