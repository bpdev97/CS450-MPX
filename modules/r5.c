#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include <system.h>
#include "mpx_supt.h"
#include "mcb.h"
#include "r5.h"

void isEmpty(){
    if(AMCB == NULL){
        println("True");
    }
    else {
        println("False");
    }
    return;
}

void showFreeMemory(){
    CMCB* memoryBlock = FMCB;       
    char string[10];
    int i = 0;
    int offset = 0;
    println("UNALLOCATED MEMORY");
    println("--------------");
    while (memoryBlock != NULL){
        offset = (int) memoryBlock - (int) HEAP;
        print("Memory Block: ");
        println(itoa(i, string, 10));
        println("--------------");
        memset(string, 0, 10);
        print("Offset: ");
        println(itoa(offset, string, 10));
        memset(string, 0, 10);
        print("Size: ");
        println(itoa(memoryBlock -> size, string, 10));
        memset(string, 0, 10);
        println(" ");
        memoryBlock = memoryBlock -> next;
        i++;
    }
    return;
}

void showAllocatedMemory(){
    CMCB* memoryBlock = AMCB;       
    char string[10];
    int i = 0;
    int offset = 0;
    println("ALLOCATED MEMORY");
    println("--------------");
    while (memoryBlock != NULL){
        offset = (int) memoryBlock - (int) HEAP;
        print("Memory Block: ");
        println(itoa(i, string, 10));
        println("--------------");
        memset(string, 0, 10);
        print("Offset: ");
        println(itoa(offset, string, 10));
        memset(string, 0, 10);
        print("Size: ");
        println(itoa(memoryBlock -> size, string, 10));
        memset(string, 0, 10);
        println(" ");
        memoryBlock = memoryBlock -> next;
        i++;
    }
    return;
}

void* allocateMemory(int size){
    // ERROR size cannot be <= 0 - return (void*) -1
    if(size <= 0){
        println("Allocation failed: size too small");
        return (void*) -1;
    }

    void* mem = allocMem(size);
    if(!mem){
        println("Allocation Failed.");
    }
    else{
        char string[10];
        println("Allocation Succeeded.");
        print("Offset Address: ");
        println(itoa((int) mem - (int) HEAP - sizeof(CMCB), string, 10));
        print("Actual Address (decimal): ");
        println(itoa((int) mem, string, 10));
    }
    return mem;
}

void freeMemory(int address){
    int error = freeMem((void*) address);
    char string[10];
    if(error == -1){
        println("Free memory failed.");
    }
    else{
        println("Free memeory succeeded.");
    }
    print("Offset Address: ");
    println( itoa(address - (int) HEAP - sizeof(CMCB), string, 10));
    return;
}