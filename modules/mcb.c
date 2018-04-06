
#include "mcb.h"
#include "mpx_supt.h"
#include "mem/heap.h"
#include <system.h>

CMCB* AMCB;
CMCB* FMCB;
LMCB* END;
void* start;

int MIN_FREE_SIZE = 128;

int initializeHeap(int bytes){
    //All memory blocks are rounded up to next full word (4 bytes)
    int mod = bytes%4;
    if(mod > 0){
        bytes = bytes + (4 - mod);
    }
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

void* allocMem(int size){
    //Make size a mulitple of 4 bytes (word)
    int mod = size%4;
    if(mod > 0){
        size = size + (4 - mod);
    }
    
    //Traverse free list until a block of large enough size is found
    //If no suitable block is found, do nothing and return NULL
    CMCB* current = FMCB;
    while(current -> size < (int) (size + sizeof(CMCB) + sizeof(LMCB))){
        if(current -> next == NULL){
            return NULL;
        }
        current = current -> next;
    }

    //First unlink the FMCB (current)
    //I think it should be fine if any of these things are NULL but we should check that
    current -> previous -> next = current -> next;
    current -> next -> previous = current -> previous;
    current -> next = NULL;
    current -> previous = NULL;

    /*
     * Write a CMCB (allocated) at the beginning of the block created and an LMCB at the end
     * Insert into AMCB list in order of increasing address
     * If the remainder block would be less than MIN_FREE_SIZE, upgrade size to use the full block
     * and don't create a remainder
    */

    //No remainder case
    //Insert into FMCB list in order of increasing address
    if((int) (current -> size - size - sizeof(CMCB) - sizeof(LMCB)) < MIN_FREE_SIZE){
        size = current -> size;
        current -> type = 1;

        //Get a pointer for the LMCB at the end of the block and change its type to 1 (allocated)
        LMCB* endCap = (LMCB*) current -> beginning + size;
        endCap -> type = 1;
        
        insertMCB(current);
    }
    
    //Remainder case
    //Create remainder block. CMCB (free) at beginning and LMCB at the end
    //Insert into FMCB list in order of increasing address
    else{
        //Change type to allocated
        current -> type = 1;

        //Create new LMCB for the allocated block
        LMCB* endCap = current -> beginning + size;
        endCap -> type = 1;
        endCap -> size = size;

        //Create new FMCB
        CMCB* newFree = (CMCB*) endCap + sizeof(LMCB);
        newFree -> type = 0;
        newFree -> size = current -> size - size - sizeof(CMCB) - sizeof(LMCB);

        //Update the very end of the heap to reflect the change in memory
        END -> size = END -> size - size - sizeof(CMCB) - sizeof(LMCB);
        
        current -> size = size;

        insertMCB(current);
        insertMCB(newFree);
    }
    
    return current -> beginning;
}

int freeMem(void* ptr){
    // Search allocated list for AMCB representing block to be freed
    CMCB* current = AMCB;
    while(current != NULL && current -> beginning != ptr) {
        current = current -> next;
    }

    // Couldn't find the AMCB or it wasn't already allocated - return -1
    if(current == NULL) {
        return -1;
    }

    // Unlink the AMCB
    unlinkMCB(current);

    // Get currents LMCB
    LMCB* endCap = (LMCB*) current -> beginning + current -> size;

    // Look for adjacent free block after
    CMCB* fABlockAfter = (CMCB*) endCap + sizeof(LMCB);
    CMCB* fABlockCurrentAfter = AMCB;
    LMCB* endCapAfter = (LMCB*) fABlockAfter -> beginning + fABlockAfter -> size;
    
    while(fABlockCurrentAfter != NULL && fABlockCurrentAfter != fABlockAfter){
        fABlockCurrentAfter = fABlockCurrentAfter -> next;
    }

    // Look for adjacent free block before
    CMCB* fABlockBefore = (CMCB*) endCap + sizeof(LMCB);
    CMCB* fABlockCurrentBefore = AMCB;
    
    while(fABlockCurrentBefore != NULL && fABlockCurrentBefore != fABlockBefore){
        fABlockCurrentBefore = fABlockCurrentBefore -> next;
    }

    // No adjacent blocks
    if(fABlockCurrentAfter == NULL && fABlockCurrentBefore == NULL){
        current -> type = 0;
        endCap -> type = 0;
        insertMCB(current);
        return 0;
    }

    // adjacent after
    if(fABlockCurrentAfter != NULL && fABlockCurrentBefore == NULL){
        current -> type = 0;
        unlinkMCB(fABlockAfter);
        current -> size = current -> size + sizeof(LMCB) + sizeof(CMCB) + fABlockAfter -> size;
        endCapAfter -> size = current -> size;
        endCapAfter -> type = 0;
        insertMCB(current);
        return 0;
    }

    // adjacent before
    else if(fABlockCurrentAfter == NULL && fABlockCurrentBefore != NULL){
        unlinkMCB(fABlockBefore);
        fABlockBefore -> type = 0;
        fABlockBefore -> size = fABlockBefore -> size + sizeof(LMCB) + sizeof(CMCB) + current -> size;
        endCap -> size = fABlockBefore -> size;
        endCap -> type = 0;
        insertMCB(fABlockBefore);
        return 0;
    }

    // adjacent before and after
    else {
        unlinkMCB(fABlockBefore);
        unlinkMCB(fABlockAfter);
        fABlockBefore -> type = 0;
        fABlockBefore -> size = fABlockBefore -> size + (2 * sizeof(LMCB)) + (2 * sizeof(CMCB)) + fABlockAfter -> size + current -> size;
        endCapAfter -> type = 0;
        endCapAfter -> size = fABlockBefore -> size;
        insertMCB(fABlockBefore);
        return 0;
    }
}

// Funtion for handling unlinking logic
void unlinkMCB(CMCB* mcb){
    // mcb is a FMCB and only item in the list
    if(mcb -> type == 0 && mcb -> next == NULL && mcb -> previous == NULL){
        FMCB = NULL;
        return;
    }

    // mcb is a AMCB and only item in the list
    else if (mcb -> type == 1 && mcb -> next == NULL && mcb -> previous == NULL){
        AMCB = NULL;
        return;
    }
        
    // last item in the list
    if(mcb -> next == NULL){
        mcb -> previous -> next = NULL;
        mcb -> previous = NULL;
        return;
    }

    // somewhere inbetween
    mcb -> next -> previous = mcb -> previous;
    mcb -> previous -> next = mcb -> next;
    mcb -> next = NULL;
    mcb -> previous = NULL;
    return;
}

// Funtion for handling insertion logic
void insertMCB(CMCB* mcb){
    //Check what type of mcb is being inserted
    CMCB* list = NULL;
    if(mcb -> type == 0){
        list = FMCB;
    }
    else{
        list = AMCB;
    }

    //If the list is empty just set it to the head
    if(list == NULL){
        mcb -> next = NULL;
        mcb -> previous = NULL;
        if(mcb -> type == 0){
            FMCB = mcb;
        }
        else{
            AMCB = mcb;
        }
        return;
    }
    
    //Traverse the list until current -> next is a higher address than mcb
    CMCB* current = list;
    while(current -> next < mcb && current -> next){
        current = current -> next;
    }

    /*
        * Link mcb -> next to what current pointed to
        * Link mcb -> previous to current
        * Link the mcb after mcb to point back to mcb
        * Link current to point forward to mcb
    */
    mcb -> next = current -> next;
    mcb -> previous = current;
    mcb -> next -> previous = mcb;
    current -> next = mcb;

    return;
}

