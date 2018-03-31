
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
    ptr = ptr;
    return 0;
}

//Funtion for handling insertion logic
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

