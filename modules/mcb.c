
#include "mcb.h"
#include "mpx_supt.h"
#include "mem/heap.h"
#include <system.h>

CMCB* AMCB;
CMCB* FMCB;
LMCB* END;
void* start;

int initializeHeap(int bytes){
    start = (void*) kmalloc(bytes + sizeof(CMCB) + sizeof(LMCB));

    AMCB = NULL;
    FMCB = start;
    FMCB -> previous = NULL;
    FMCB -> next = NULL;
    END = start + bytes + sizeof(CMCB);

    return bytes;
}

