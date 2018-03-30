
#ifndef _MCB_H
#define _MCB_H

#include <system.h>

typedef struct cmcb{
    int type;
    int* beginning;
    int size;
    char* processName;
    struct cmcb* next;
    struct cmcb* previous;
} CMCB;

typedef struct lmcb{
    int type;
    int size;
} LMCB;

int initializeHeap(int bytes);
void* allocMem(u32int size);
int freeMem(void* ptr);

#endif