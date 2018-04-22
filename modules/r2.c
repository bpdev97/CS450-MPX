#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include <system.h>
#include "mpx_supt.h"
#include "pcb.h"
#include "queue.h"
#include "r2.h"

void showPCB(int argc, char* argv[]){
    // User did not specify PCB
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

    else {
        print("Process Name: "); 
        println(currentPCB -> name); 

        print("Class: "); 
        itoa((currentPCB -> classNum), classNumber, 10); //converts class int to char *
        println(classNumber);

        if(currentPCB->readyState == 1){
            println("State: Ready");  
        }
        else{
            println("State: Blocked");  
        }

        if(currentPCB -> suspendState == 1){
            println("Suspended Status: Suspended ");
        }
        else{
            println("Suspended Status: Not Suspended");
        }

        print("Priority: ");
        itoa((currentPCB -> priority), priorityNumber, 10);
        println(priorityNumber);

        println(" ");
    }
}

//display all PCB info in ready queues
void showReady(){
    PCB* currentPCB = ready -> head;
    char classNumber[2];
    char priorityNumber[2];

    int i;

    for(i = 0; i< ready -> count; i++) {
        print("Process Name: "); 
        println(currentPCB -> name); 

        print("Class: "); 
        itoa((currentPCB -> classNum),classNumber, 10); //converts class int to char *
        println(classNumber);

        if(currentPCB -> readyState == 1){
            println("State: Ready");  
        }
        else{
            println("State: Blocked");  
        }

        if(currentPCB -> suspendState == 1){
            println("Suspended Status: Suspended ");
        }
        else{
            println("Suspended Status: Not Suspended");
        }

        print("Priority: ");
        itoa((currentPCB -> priority),priorityNumber, 10);//converts priority int to char *
        println(priorityNumber);

        println(" ");
        currentPCB = currentPCB -> nextPcb;
    }

    currentPCB = readySuspended -> head;

    for(i = 0; i< readySuspended -> count; i++) {
        print("Process Name: "); 
        println(currentPCB->name); 

        print("Class: "); 
        itoa((currentPCB -> classNum),classNumber, 10); //converts class int to char *
        println(classNumber);

        if(currentPCB -> readyState == 1){
            println("State: Ready");  
        }
        else{
            println("State: Blocked");  
        }

        if(currentPCB -> suspendState == 1){
            println("Suspended Status: Suspended ");
        }
        else{
            println("Suspended Status: Not Suspended");
        }

        print("Priority: ");
        itoa((currentPCB -> priority), priorityNumber, 10);//converts priority int to char *
        println(priorityNumber);

        println(" ");
        currentPCB = currentPCB -> nextPcb;
    }
}

//display all PCB info in blocked queues
void showBlocked(){
    PCB* currentPCB = blocked -> head;
    char classNumber[2];
    char priorityNumber[2];

    int i;

    for(i = 0; i< blocked -> count; i++){
        print("Process Name: "); 
        println(currentPCB -> name); 

        print("Class: "); 
        itoa((currentPCB -> classNum),classNumber, 10); //converts class int to char *
        println(classNumber);

        if(currentPCB -> readyState == 1){
            println("State: Ready");  
        }
        else{
            println("State: Blocked");  
        }

        if(currentPCB -> suspendState == 1){
            println("Suspended Status: Suspended ");
        }
        else{
            println("Suspended Status: Not Suspended");
        }

        print("Priority: ");
        itoa((currentPCB -> priority), priorityNumber, 10);//converts priority int to char *
        println(priorityNumber);

        println(" ");
        currentPCB = currentPCB -> nextPcb;
    }

    currentPCB = blockedSuspended -> head; 

    for(i = 0; i< blockedSuspended -> count; i++){
        print("Process Name: "); 
        println(currentPCB -> name); 

        print("Class: "); 
        itoa((currentPCB -> classNum),classNumber, 10); //converts class int to char *
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
        itoa((currentPCB -> priority),priorityNumber, 10);//converts priority int to char *
        println(priorityNumber);

        println(" ");
        currentPCB = currentPCB -> nextPcb;
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