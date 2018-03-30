
#include "mcb.h"
#include "mpx_supt.h"
#include "mem/heap.h"
#include <system.h>

CMCB* AMCB;
CMCB* FMCB;
LMCB* END;
void* start;

u32int minSize = 128;

int initializeHeap(int bytes){
    //Beginning of the entire heap
    start = (void*) kmalloc(bytes + sizeof(CMCB) + sizeof(LMCB));

    if(start == NULL){
        return -1; //Error code
    }

    //Allocated list head initialized
    AMCB = NULL;

    //Free list head initialized
    FMCB -> type = 0;
    FMCB = start;
    FMCB -> size = bytes;
    FMCB -> beginning = FMCB + sizeof(CMCB);
    FMCB -> previous = NULL;
    FMCB -> next = NULL;

    END = start + bytes + sizeof(CMCB);
    END -> type = 0;
    END -> size = bytes;

    return bytes;
}

void* allocMem(u32int size){
    //Check for minimum size
    if(size < minSize){
        size = minSize;
    }

    //Make size a mulitple of 4 bytes (word)
    int mod = size%4;
    if(mod > 0){
        size = size + (4 - mod);
    }
    
    CMCB* current = FMCB;

    while(current -> size < size + sizeof(CMCB) + sizeof(LMCB)){
        if(current -> next == NULL){
            return NULL;
        }

        current = current -> next;
    }
    
}

