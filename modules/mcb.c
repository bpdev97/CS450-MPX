#include "mcb.h"
#include "mpx_supt.h"
#include "mem/heap.h"
#include <system.h>

CMCB* AMCB = NULL;
CMCB* FMCB = NULL;
LMCB* END = NULL;
void* startMem = NULL;
void* HEAP = NULL;

int MIN_FREE_SIZE = 128;

int initializeHeap(int bytes){
    // All memory blocks are rounded up to next full word (4 bytes)
    int mod = bytes%4;
    if(mod > 0){
        bytes = bytes + (4 - mod);
    }
    
    // Beginning of the entire heap
    startMem = (void*) kmalloc(bytes + sizeof(CMCB) + sizeof(LMCB));

    // ERROR: kmalloc failed to allocate memory
    if(startMem == NULL){
        return -1;
    }

    // Initialize head of Allocated MCB list
    AMCB = NULL;

    // Initialize head of Free MCB list
    FMCB = startMem;
    FMCB -> type = 0;
    FMCB -> size = bytes;
    FMCB -> beginning = (void*) ((int) FMCB + sizeof(CMCB));
    FMCB -> previous = NULL;
    FMCB -> next = NULL;

    // Set HEAP to first Free MCB
    HEAP = FMCB;

    // Initialize END of heap
    END = (LMCB*) ((int) startMem + bytes + sizeof(CMCB));
    END -> type = 0;
    END -> size = FMCB -> size;
    
    return bytes;
}

u32int allocMem(u32int size){
    // Make size a mulitple of 4 bytes (word)
    int mod = size%4;
    if(mod > 0){
        size = size + (4 - mod);
    }
    
    // Traverse free list until a block of large enough size is found
    CMCB* current = FMCB;
    while(current -> size < (int) size){
        // ERROR: no suitable block found - return NULL
        if(current -> next == NULL){
            return NULL;
        }
        current = current -> next;
    }

    // Unlink the FMCB (current)
    unlinkMCB(current);

    /*
     * Write a CMCB (allocated) at the beginning of the block created and an LMCB at the end
     * Insert into AMCB list in order of increasing address
     * If the remainder block would be less than MIN_FREE_SIZE, upgrade size to use the full block
     * and don't create a remainder
     *  This is fine - Alex
    */

    // No remainder case (More space is given than requested)
    // Insert into FMCB list in order of increasing address
    if((int) (current -> size - size - sizeof(CMCB) - sizeof(LMCB)) < MIN_FREE_SIZE){
        size = current -> size;
        current -> type = 1;

        // Get a pointer for the LMCB at the end of the block and change its type to 1 (allocated)
        LMCB* endCap = (LMCB*) (((int) current -> beginning) + size);
        endCap -> type = 1;

        // Update the very end of the free area of the heap to reflect the change in memory
        END -> size = END -> size - size - sizeof(CMCB) - sizeof(LMCB);
        current -> beginning = (void*) (current + sizeof(CMCB));
        
        insertMCB(current);
    }
    
    // Remainder case
    // Create remainder block. Free CMCB at beginning and LMCB at the end
    // Insert into FMCB list in order of increasing address
    else{
        // Change type to allocated
        current -> type = 1;

        // Create new LMCB for the allocated block
        LMCB* endCap = (LMCB*) ((int) (current -> beginning) + size);
        endCap -> type = 1;
        endCap -> size = size;

        // Create new FMCB
        CMCB* newFree = (CMCB*) ((int) (endCap) + sizeof(LMCB));
        newFree -> type = 0;
        newFree -> size = current -> size - size - sizeof(CMCB) - sizeof(LMCB);
        newFree -> beginning = (void*) ((int) newFree + sizeof(CMCB));

        // Update the very end of the free area of the heap to reflect the change in memory
        END -> size = END -> size - size;
        
        current -> size = size;

        insertMCB(current);
        insertMCB(newFree);
    }
    
    return (u32int) current -> beginning;
}

int freeMem(void* ptr){
    // Search allocated list for AMCB representing block to be freed
    CMCB* current = AMCB;
    while(current != NULL && current -> beginning != ptr) {
        current = current -> next;
    }

    // ERROR: Couldn't find the AMCB or it wasn't already allocated - return -1
    if(current == NULL) {
        return -1;
    }

    // Unlink the AMCB
    unlinkMCB(current);

    // Get currents LMCB
    LMCB* endCap = (LMCB*) (((int) current -> beginning) + current -> size);

    // Look for adjacent free block after
    CMCB* blockAfter = (CMCB*) ((int) endCap + sizeof(LMCB));
    LMCB* endCapAfter = (LMCB*) (((int) blockAfter -> beginning) + blockAfter -> size);
    CMCB* currentAfter = FMCB;
    
    while(currentAfter != NULL && currentAfter != blockAfter){
        currentAfter = currentAfter -> next;
    }

    // Look for adjacent free block before
    LMCB* endCapBefore = (LMCB*) ((int) current - sizeof(LMCB));
    CMCB* blockBefore = (CMCB*) ((int) endCapBefore - endCapBefore -> size - sizeof(CMCB));
    CMCB* currentBefore = FMCB;
    
    while(currentBefore != NULL && currentBefore != blockBefore){
        currentBefore = currentBefore -> next;
    }

    // No adjacent blocks
    if(currentAfter == NULL && currentBefore == NULL){
        current -> type = 0;
        endCap -> type = 0;
        insertMCB(current);
        return 0;
    }

    // adjacent after
    if(currentAfter != NULL && currentBefore == NULL){
        current -> type = 0;
        unlinkMCB(blockAfter);
        current -> size = current -> size + sizeof(LMCB) + sizeof(CMCB) + blockAfter -> size;
        endCapAfter -> size = current -> size;
        endCapAfter -> type = 0;
        insertMCB(current);
        return 0;
    }

    // adjacent before
    else if(currentAfter == NULL && currentBefore != NULL){
        unlinkMCB(blockBefore);
        blockBefore -> type = 0;
        blockBefore -> size = blockBefore -> size + sizeof(LMCB) + sizeof(CMCB) + current -> size;
        endCap -> size = blockBefore -> size;
        endCap -> type = 0;
        insertMCB(blockBefore);
        return 0;
    }

    // adjacent before and after
    else {
        unlinkMCB(blockBefore);
        unlinkMCB(blockAfter);
        blockBefore -> type = 0;
        blockBefore -> size = blockBefore -> size + (2 * sizeof(LMCB)) + (2 * sizeof(CMCB)) + blockAfter -> size + current -> size;
        endCapAfter -> type = 0;
        endCapAfter -> size = blockBefore -> size;
        insertMCB(blockBefore);
        return 0;
    }
}

// Funtion for handling unlinking logic
void unlinkMCB(CMCB* mcb){
    // unlinking head
    if(mcb == FMCB){
        FMCB = mcb -> next;
        mcb -> next = NULL;
        mcb -> previous = NULL;
        FMCB -> previous = NULL;
        return;
    }

    else if(mcb == AMCB){
        AMCB = mcb -> next;
        mcb -> next = NULL;
        mcb -> previous = NULL;
        AMCB -> previous = NULL;
        return;
    }

    // mcb is a Free and the only item in the list then set FMCB to NULL
    if(mcb -> type == 0 && mcb -> next == NULL && mcb -> previous == NULL){
        FMCB = NULL;
        return;
    }

    // mcb is a Allocated and only item in the list then set AMCB to NULL
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

    // somewhere in between
    mcb -> next -> previous = mcb -> previous;
    mcb -> previous -> next = mcb -> next;
    mcb -> next = NULL;
    mcb -> previous = NULL;
    return;
}

// Funtion for handling insertion logic
void insertMCB(CMCB* mcb){
    // Check what type of mcb is being inserted
    CMCB* list = NULL;
    if(mcb -> type == 0){
        list = FMCB;
    }
    else{
        list = AMCB;
    }

    // If the list is empty just set it to the head
    if(list == NULL){
        if(mcb -> type == 0){
            FMCB = mcb;
        }
        else{
            AMCB = mcb;
        }
        return;
    }

    // Traverse the list 
    CMCB* current = list;
    
    // New head
    if((int) mcb < (int) current){
        mcb -> next = current;
        mcb -> next -> previous = mcb;
        if (mcb -> type == 0){
            FMCB = mcb;
        }
        else{
            AMCB = mcb;
        }
        return;
    }

    // Traverse the list
    while((int) mcb > (int) current && current -> next != NULL){
        current = current -> next;
    }

    // New tail
    if(current -> next == NULL && (int) mcb > (int) current){
        current -> next = mcb;
        mcb -> previous = current;
        return;
    }

    // in between - mcb < current
    mcb -> next = current;
    mcb -> previous = current -> previous;
    current -> previous -> next = mcb;
    current -> previous = mcb;
    return;
}