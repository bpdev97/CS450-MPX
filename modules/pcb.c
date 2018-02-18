#include <system.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include "processcommand.h"
#include "queue.h"
#include "pcb.h"


PCB* AllocatePCB(){
    PCB* pointpcb = sys_alloc_mem(sizeof(PCB));
    unsigned char* pointstack = sys_alloc_mem(1024); //Stack Size?
    // ^ This has to be an unsigned char -Addison
    if(pointpcb == NULL || pointstack == NULL){
        return NULL;
    }
    else{
        pointpcb -> stack = pointstack;
        return pointpcb;
    }
}

int FreePCB(PCB* freepcb){
    int success = sys_free_mem(freepcb -> stack);
    int error = sys_free_mem(freepcb);

    if(success == 1 && error == 1){
        return 1;
    }
    else{
        return 0;
    }
}

PCB* SetupPCB(char* name, int classNum, int priority){
    PCB* pointpcb = AllocatePCB();
    pointpcb -> name = name;
    if(classNum == 1 || classNum == 0){
        pointpcb -> classNum = classNum;
    }
    else{
        return NULL;
    }
    if(priority >= 0 && priority <= 9){
        pointpcb -> priority = priority;
    }
    else{
        return NULL;
    }
    return pointpcb;
}

PCB *FindPCB(const char *name){
    // Traverse queues
    // Ready queue
    PCB* currentPCB = ready -> head;
    while(currentPCB != ready -> tail){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

    // ReadySuspended queue
    currentPCB = readySuspended -> head;
    while(currentPCB != readySuspended -> tail){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

    // Blocked queue
    currentPCB = blocked -> head;
    while(currentPCB != blocked -> tail){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

     // BlockedSuspended queue
    currentPCB = blockedSuspended -> head;
    while(currentPCB != blockedSuspended -> tail){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

    return NULL;
}

void InsertPCB(PCB *p){
    return;
}

int RemovePCB(PCB *p){
    return NULL;
}

