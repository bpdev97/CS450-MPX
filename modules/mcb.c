
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

