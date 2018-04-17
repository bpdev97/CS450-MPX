#include <system.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include "r2.h"
#include "queue.h"
#include "pcb.h"


PCB* AllocatePCB(){
    PCB* pointpcb = sys_alloc_mem(sizeof(PCB));
    // probably should memset here
    pointpcb -> stackBase = sys_alloc_mem(1024);
    pointpcb -> stackTop = pointpcb -> stackBase - sizeof(CONTEXT) + 1024;
    pointpcb -> context = (CONTEXT*) pointpcb -> stackTop;
    return pointpcb;
}

int FreePCB(PCB* freepcb){
    int success = sys_free_mem(freepcb -> stackBase);
    sys_free_mem(freepcb -> name);
    int error = sys_free_mem(freepcb);
    
    if(success == 1 && error == 1){
        return 1;
    }
    else{
        return 0;
    }
}

PCB* SetupPCB(char* name, int classNum, int priority, void* function){
    PCB* pointpcb = AllocatePCB();
    pointpcb -> name = sys_alloc_mem(strlen(name) + 1);
    strcpy(pointpcb -> name, name);
    pointpcb -> readyState = 1;
    pointpcb -> suspendState = 0;
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
    
    //initializing context values
    pointpcb -> context -> fs = 0x10;
    pointpcb -> context -> gs = 0x10;
    pointpcb -> context -> ds = 0x10;
    pointpcb -> context -> es = 0x10;
    pointpcb -> context -> cs = 0x8;
    pointpcb -> context -> edi = 0x0;
    pointpcb -> context -> esi = 0x0;
    pointpcb -> context -> ebx = 0x0;
    pointpcb -> context -> edx = 0x0;
    pointpcb -> context -> ecx = 0x0;
    pointpcb -> context -> eax = 0x0;
    pointpcb -> context -> ebp = (u32int) pointpcb -> stackBase;
    pointpcb -> context -> esp = (u32int) pointpcb -> stackTop;
    pointpcb -> context -> eip = (u32int) function;
    pointpcb -> context -> eflags = 0x202;

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
    int i;
    PCB* currentPCB = ready -> head;

    // Find correct queue
    // ready - priority queue
    if(p -> readyState == 1 && p -> suspendState == 0){
        // nothing in queue
        if(ready -> count == 0){
            ready -> head = p;
            ready -> tail = p;
            ready -> count++;
            return;
        }
        // new head
        if(p -> priority < currentPCB -> priority){
            p -> nextPcb = currentPCB;
            ready -> head = p;
            ready -> count++;
            return;
        }
        // new tail
        if(p -> priority >= ready -> tail -> priority){
            ready -> tail -> nextPcb = p;
            ready -> tail = p;
            ready -> count++;
            return;
        }
        // in between
        for(i = 0; i < ready -> count; i++){
            if(p -> priority == currentPCB -> priority){
                if(p -> priority != currentPCB -> nextPcb -> priority){
                    p -> nextPcb = currentPCB -> nextPcb;
                    currentPCB -> nextPcb = p;
                    ready -> count++;
                    return;
                }
                else {
                    currentPCB = currentPCB -> nextPcb;
                }
            }
            else if(p -> priority < currentPCB -> nextPcb -> priority){
                p -> nextPcb = currentPCB -> nextPcb;
                currentPCB -> nextPcb = p;
                ready -> count++;
                return;
            }
            else {
                currentPCB = currentPCB -> nextPcb;
            }
        }
    }

    // readySuspended - priority queue
    else if(p -> readyState == 1 && p -> suspendState == 1){
        // nothing in queue
        if(readySuspended -> count == 0){
            readySuspended -> head = p;
            readySuspended -> tail = p;
            readySuspended -> count++;
            return;
        }
        currentPCB = readySuspended -> head;

        // new head
        if(p -> priority < currentPCB -> priority){
            p -> nextPcb = currentPCB;
            readySuspended -> head = p;
            readySuspended -> count++;
            return;
        }
        // new tail
        if(p -> priority >= readySuspended -> tail -> priority){
            readySuspended -> tail -> nextPcb = p;
            readySuspended -> tail = p;
            readySuspended -> count++;
            return;
        }
        // in between
        for(i = 0; i < readySuspended -> count; i++){
            if(p -> priority == currentPCB -> priority){
                if(p -> priority != currentPCB -> nextPcb -> priority){
                    p -> nextPcb = currentPCB -> nextPcb;
                    currentPCB -> nextPcb = p;
                    readySuspended -> count++;
                    return;
                }
                else {
                    currentPCB = currentPCB -> nextPcb;
                }
            }
            else if(p -> priority < currentPCB -> nextPcb -> priority){
                p -> nextPcb = currentPCB -> nextPcb;
                currentPCB -> nextPcb = p;
                readySuspended -> count++;
                return;
            }
            else {
                currentPCB = currentPCB -> nextPcb;
            }
        }
    }

    // blocked - FIFO queue
    else if(p -> readyState == -1 && p -> suspendState == 0){
        p -> nextPcb = blocked -> head;
        blocked -> head = p;
        blocked -> count++;
        return;
    }

    // blockedSuspended - FIFO queue
    else if(p -> readyState == -1 && p -> suspendState == 1){
        p -> nextPcb = blockedSuspended -> head;
        blockedSuspended -> head = p;
        blockedSuspended -> count++;
        return;
    }

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
        //FreePCB(p);
        return 0;
    }
    
    // PCB is head
    if(p == ready -> head){
        ready -> head = p -> nextPcb;
        ready -> count--;
        //FreePCB(p);
        return 0;
    }

    int i;
    PCB* currentPCB = ready -> head;
    for(i = 0; i < ready -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            ready -> count--;
            //FreePCB(p);
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
        //FreePCB(p);
        return 0;
    }
    
    // PCB is head
    if(p == readySuspended -> head){
        readySuspended -> head = p -> nextPcb;
        readySuspended -> count--;
        //FreePCB(p);
        return 0;
    }

    currentPCB = readySuspended -> head;
    for(i = 0; i < readySuspended -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            readySuspended -> count--;
            //FreePCB(p);
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
        //FreePCB(p);
        return 0;
    }
    
    // PCB is head
    if(p == blocked -> head){
        blocked -> head = p -> nextPcb;
        blocked -> count--;
        //FreePCB(p);
        return 0;
    }

    currentPCB = blocked -> head;
    for(i = 0; i < blocked -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            blocked -> count--;
            //FreePCB(p);
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
        //FreePCB(p);
        return 0;
    }
    
    // PCB is head
    if(p == blockedSuspended -> head){
        blockedSuspended -> head = p -> nextPcb;
        blockedSuspended -> count--;
        //FreePCB(p);
        return 0;
    }

    currentPCB = blockedSuspended -> head;
    for(i = 0; i < blockedSuspended -> count; i++){
        if(currentPCB -> nextPcb == p) {
            currentPCB -> nextPcb = currentPCB -> nextPcb -> nextPcb;
            blockedSuspended -> count--;
            //FreePCB(p);
            return 0;
        }
        else {
            currentPCB = currentPCB -> nextPcb;
        }
    }

    // Didn't remove PCB
    return -1;
}








