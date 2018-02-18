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
    int i;
    for(i = 0; i < ready -> count; i++){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

    // ReadySuspended queue
    currentPCB = readySuspended -> head;
    for(i = 0; i < readySuspended -> count; i++){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

    // Blocked queue
    currentPCB = blocked -> head;
    for(i = 0; i < blocked -> count; i++){
        if(strcmp(currentPCB -> name, name) == 0)
            return currentPCB;
        else
            currentPCB = currentPCB -> nextPcb;
    }

     // BlockedSuspended queue
    currentPCB = blockedSuspended -> head;
    for(i = 0; i < blockedSuspended -> count; i++){
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
    // Traverse queues
    // Ready queue
    // only one pcb in queue
    if(p == ready -> head && p == ready -> tail){
        ready -> head = NULL;
        ready -> tail = NULL;
        ready -> count--;
        return 0;
    }
    
    // PCB is head
    if(p == ready -> head){
        ready -> head = p -> nextPcb;
        ready -> count--;
        return 0;
    }

    int i;
    PCB* currentPCB = ready -> head;
    for(i = 0; i < ready -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            ready -> count--;
            return 0;
        }
        else {
            currentPCB = currentPCB -> nextPcb;
        }
    }

    // readySuspended queue
    // only one pcb in queue
    if(p == readySuspended -> head && p == readySuspended -> tail){
        readySuspended -> head = NULL;
        readySuspended -> tail = NULL;
        readySuspended -> count--;
        return 0;
    }
    
    // PCB is head
    if(p == readySuspended -> head){
        readySuspended -> head = p -> nextPcb;
        readySuspended -> count--;
        return 0;
    }

    currentPCB = readySuspended -> head;
    for(i = 0; i < readySuspended -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            readySuspended -> count--;
            return 0;
        }
        else {
            currentPCB = currentPCB -> nextPcb;
        }
    }

    // blocked queue
    // only one pcb in queue
    if(p == blocked -> head && p == blocked -> tail){
        blocked -> head = NULL;
        blocked -> tail = NULL;
        blocked -> count--;
        return 0;
    }
    
    // PCB is head
    if(p == blocked -> head){
        blocked -> head = p -> nextPcb;
        blocked -> count--;
        return 0;
    }

    currentPCB = blocked -> head;
    for(i = 0; i < blocked -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            blocked -> count--;
            return 0;
        }
        else {
            currentPCB = currentPCB -> nextPcb;
        }
    }

    // blockedSuspended queue
    // only one pcb in queue
    if(p == blockedSuspended -> head && p == blockedSuspended -> tail){
        blockedSuspended -> head = NULL;
        blockedSuspended -> tail = NULL;
        blockedSuspended -> count--;
        return 0;
    }
    
    // PCB is head
    if(p == blockedSuspended -> head){
        blockedSuspended -> head = p -> nextPcb;
        blockedSuspended -> count--;
        return 0;
    }

    currentPCB = blockedSuspended -> head;
    for(i = 0; i < blockedSuspended -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            blockedSuspended -> count--;
            return 0;
        }
        else {
            currentPCB = currentPCB -> nextPcb;
        }
    }

    // Didn't remove PCB
    return -1;
}

