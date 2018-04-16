#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include <system.h>
#include "mpx_supt.h"
#include "processcommand.h"
#include "pcb.h"
#include "queue.h"
#include "mcb.h"

void showPCB(int argc, char* argv[]){
  if(argc == 1){
        println("Invalid input, did not put the PCB name.");
        return;
  }

  char* name = argv[1];
  PCB* currentPCB = FindPCB(name);
  char classNumber[2];
  char priorityNumber[2];

  if(currentPCB == NULL){
    print(name);
    println(" PCB not found!");
  }
  else{
   print("Process Name: "); 
   println(currentPCB->name); 
   
   print("Class: "); 
   itoa((currentPCB->classNum),classNumber, 10); //converts class int to char *
   println(classNumber);
   
   if(currentPCB->readyState == 1){
      println("State: Ready");  
   }
   else{
      println("State: Blocked");  
   }

   if(currentPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
   }
   else{
      println("Suspended Status: Not Suspended");
    }

   print("Priority: ");
   itoa((currentPCB->priority),priorityNumber, 10);//converts priority int to char *
   println(priorityNumber);
   
   println(" ");
  }
  
}

//display all PCB info in ready queues
void showReady(){
 PCB* currentPCB = ready->head;
 char classNumber[2];
 char priorityNumber[2];

 int i;

 for(i = 0; i< ready->count; i++)
 {
   print("Process Name: "); 
   println(currentPCB->name); 
   
   print("Class: "); 
   itoa((currentPCB->classNum),classNumber, 10); //converts class int to char *
   println(classNumber);

   if(currentPCB->readyState == 1){
      println("State: Ready");  
   }
   else{
      println("State: Blocked");  
   }

   if(currentPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
   }
   else{
      println("Suspended Status: Not Suspended");
    }

   print("Priority: ");
   itoa((currentPCB->priority),priorityNumber, 10);//converts priority int to char *
   println(priorityNumber);
   
   println(" ");
   currentPCB = currentPCB->nextPcb;
 }

  currentPCB = readySuspended->head;

for(i = 0; i< readySuspended->count; i++) {
   print("Process Name: "); 
   println(currentPCB->name); 
   
   print("Class: "); 
   itoa((currentPCB->classNum),classNumber, 10); //converts class int to char *
   println(classNumber);
   
   if(currentPCB->readyState == 1){
      println("State: Ready");  
   }
   else{
      println("State: Blocked");  
   }

   if(currentPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
   }
   else{
      println("Suspended Status: Not Suspended");
    }

   print("Priority: ");
   itoa((currentPCB->priority),priorityNumber, 10);//converts priority int to char *
   println(priorityNumber);
   
   println(" ");
   currentPCB = currentPCB->nextPcb;
 }
}


//display all PCB info in blocked queues
void showBlocked(){
 PCB* currentPCB = blocked->head;
 char classNumber[2];
 char priorityNumber[2];

 int i;

 for(i = 0; i< blocked->count; i++)
 {
   print("Process Name: "); 
   println(currentPCB->name); 
   
   print("Class: "); 
   itoa((currentPCB->classNum),classNumber, 10); //converts class int to char *
   println(classNumber);
   
   if(currentPCB->readyState == 1){
      println("State: Ready");  
   }
   else{
      println("State: Blocked");  
   }

   if(currentPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
   }
   else{
      println("Suspended Status: Not Suspended");
    }

   print("Priority: ");
   itoa((currentPCB->priority),priorityNumber, 10);//converts priority int to char *
   println(priorityNumber);
   
   println(" ");
   currentPCB = currentPCB->nextPcb;
 }

currentPCB = blockedSuspended->head; 

for(i = 0; i< blockedSuspended->count; i++)
 {
   print("Process Name: "); 
   println(currentPCB->name); 
   
   print("Class: "); 
   itoa((currentPCB->classNum),classNumber, 10); //converts class int to char *
   println(classNumber);
   
   if(currentPCB->readyState == 1){
      println("State: Ready");  
   }
   else{
      println("State: Blocked");  
   }

   if(currentPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
   }
   else{
      println("Suspended Status: Not Suspended");
    }

   print("Priority: ");
   itoa((currentPCB->priority),priorityNumber, 10);//converts priority int to char *
   println(priorityNumber);
   
   println(" ");
   currentPCB = currentPCB->nextPcb;
 }
}

//displays all PCB info
void showAll(){
  showReady();
  showBlocked();
}

//Finds a PCB and sets its state to blocked, then reinserts
int blockPCB(int argc, char *argv[]){
    PCB *find;
    if(argc == 1){
        println("Error: no process name given.");
        return 0;
    }
    
    else{
        find = FindPCB(argv[1]);
    }

    if(!find){
        println("PCB not found.");
        return 0;
    }

    else{
        RemovePCB(find);
        find -> readyState = -1;
        InsertPCB(find);
        return 1;
    }
}

//Finds a PCB and sets its state to unblocked, then reinserts
int unblockPCB(int argc, char *argv[]){
    PCB *find;
    if(argc == 1){
        println("Error: no process name given.");
        return 0;
    }

    else{
        find = FindPCB(argv[1]);
    }

    if(!find){
        println("PCB not found.");
        return 0;
    }

    else{
        RemovePCB(find);
        find -> readyState = 1;
        InsertPCB(find);
        return 1;
    }
}

//Finds a PCB and sets its state to suspended, then reinserts
int suspendPCB(int argc, char *argv[]){
    PCB *find;
    if(argc == 1){
        println("Error: no process name given.");
        return 0;
    }

    else{
        find = FindPCB(argv[1]);
    }

    if(!find){
        println("PCB not found.");
        return 0;
    }

    else{
        RemovePCB(find);
        find -> suspendState = 1;
        InsertPCB(find);
        return 1;
    }
}

//Finds a PCB and sets its state to resumed, then reinserts
int resumePCB(int argc, char *argv[]){
    PCB *find;
    if(argc == 1){
        println("Error: no process name given.");
        return 0;
    }

    else{
        find = FindPCB(argv[1]);
    }

    if(!find){
        println("PCB not found.");
        return 0;
    }

    else{
        RemovePCB(find);
        find -> suspendState = 0;
        InsertPCB(find);
        return 1;
    }
}

//Finds PCB, sets priority, reinserts
int setPriority(int argc, char *argv[]){
    PCB *find;
    if(argc < 3){
        println("Error: not enough inputs.");
        return 0;
    }

    else{
        find = FindPCB(argv[1]);
    }

    if(!find){
        println("PCB not found.");
        return 0;
    }

    int p = atoi(argv[2]);
    if(p < 0 || p > 9){
        println("Error: invalid priority.");
        return 0;
    }

    else{
        RemovePCB(find);
        find -> priority = p;
        InsertPCB(find);
        return 1;
    }
}

// Deletes the PCB
void DeletePCB(int argc, char *argv[]){
    // Checks if argc is the appropriate value
    if(argc != 2){
        println("Error, not in range.");
        return;
    }
    // Sets parameter to argv
    char* name = argv[1];

    // Error checks name
    if(strlen(name) < 8){
        println("Error, name has to be AT LEAST 8 characters.");
        return;
    }

    if(FindPCB(name) == NULL){
        println("Error, name does not exist.");
        return;
    }
    //Removes and then frees the PCB
    PCB* pcb =  FindPCB(name);
    RemovePCB(pcb);
    FreePCB(pcb);
}

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
    if(size <= 0)
    {
        println("Allocation failed: size too small");
        return;
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
