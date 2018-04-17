
#ifndef _MCB_H
#define _MCB_H

#include <system.h>

typedef struct cmcb{
    //0 for free
    //1 for allocated
    int type;
    //beginning address of the block
    void* beginning;

    //number of bytes in the block
    int size;

    //How will this struct ever get passed a name?
    char* processName;
    struct cmcb* next;
    struct cmcb* previous;
} CMCB;

typedef struct lmcb{
    int type;
    int size;
} LMCB;

extern CMCB* AMCB;
extern CMCB* FMCB;
extern LMCB* END;
extern void* HEAP;

int initializeHeap(int bytes);
u32int allocMem(u32int size);
int freeMem(void* ptr);

void insertMCB(CMCB* mcb);
void unlinkMCB(CMCB* mcb);

#endif