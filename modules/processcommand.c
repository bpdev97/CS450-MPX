
#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include <system.h>
#include "mpx_supt.h"
#include "processcommand.h"
#include "pcb.h"
#include "queue.h"

void help (int argc, char* argv[]) {

    //checking array size and setting up the Help menu
    if (argc == 1) {
        println("To use the help command type 'help' followed by the command you would like to use.");
        println("The commands you can use include: clear, shutdown, setdate, getdate, settime, gettime, version, showPCB, showReady, showBlocked, and showAll.");
    }
    //listing each command and what they do
    else if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            println("The shutdown command shuts down the operating system. Shutdown takes no arguments and will prompt the user to enter 'Y' or 'N' to choose whether to really shutdown or not. The shutdown command can be used by typing shutdown.");
        } 
        
        else if (strcmp(argv[1], "clear") == 0) {
            println("The clear command effectively clears the terminal display. The clear command prints out many new line characters to move past outputs off the screen. The clear command can be used by typing clear.");
        }        

        else if (strcmp(argv[1], "setdate") == 0) {
            println("The setdate command allows the user to input and set the system date. The setdate command follows American Month/Day/Year format. For example, today is the first of February so the input would look like: setdate 2/1/18 or setdate 02/01/18.");
        } 
        
        else if (strcmp(argv[1], "getdate") == 0) {
            println("The getdate command will print out the current date. The getdate command can be used by simply typing getdate.");
        } 
        
        else if (strcmp(argv[1], "settime") == 0) {
            println("The settime command allows the user to input and set the system time. Settime only works with 24 hour time and the hour, minute, and second must all be entered for the input to be valid. For example: settime 20:24:28 will set the time to the equivalent of 8:24:28 PM.");
        } 
        
        else if (strcmp(argv[1], "gettime") == 0) {
            println("The gettime command will retrieve and print out the current system time in 24 hour format. The gettime command can be used by typing gettime.");
        } 
        
        else if (strcmp(argv[1], "version") == 0) {
            println("The version command displays the current version of MPX and the completion date. The version command can be used by typing version.");
        }

        else if (strcmp(argv[1], "showPCB") == 0) {
            println("The showPCB checks to see if the PCB name exists in the in the ready queue, readySuspended queue, blocked queue, or blockedSuspended queue. If the PCB exists then the name, class, ready state, blocked state, and priority of the PCB is displayed. The showPCB command can be used by typing showPCB followed by the name of the PCB. For example 'showPCB testName' will search the queues for a PCB with the name testName.");
        }
        else if (strcmp(argv[1], "showReady") == 0) {
            println("The showReady command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue and readySuspended queue. The showReady command can be used by typing showReady.");
        }
        else if (strcmp(argv[1], "showBlocked") == 0) {
            println("The showBlocked command displays the name, class, ready state, blocked state, and priority for all PCBs in the blocked queue and blockedSuspended queue. The showBlocked command can be used by typing showBlocked.");
        }
        else if (strcmp(argv[1], "showAll") == 0) {
            println("The showAll command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue, readySuspended queue, blocked queue, and blockedSuspended queue. The showAll command can be used by typing showAll.");
        }
        else if (strcmp(argv[1], "createPCB") == 0) {
            println("The createPCB command will call SetupPCB() and insert the PCB in the appropriate queue.");
        }
        else if (strcmp(argv[1], "deletePCB") == 0) {
            println("The deletePCB command will remove a PCB from the appropriate queue and then free all associated memory..");
        }

     //error checking
        else{
            println("Invalid input, command not found.");
        }
    }
    else{
        println("Invalid input, too many arguments.");
    }
}

void version (int argc, char* argv[]) {
    println("Version: R2.");
}

//allows user to set the time
void settime (int argc, char* argv[]) {
    cli();

    //checking array size and error checking
    if(argc == 1){
        println("Invalid input, did not put the time.");
        return;
    }

    //checking if hour is valid
    char* hour = strtok(argv[1], ":"); //ERROR
    int hours = atoi(hour);
    if(hours < 0 || hours > 24){
        println("Invalid input, hour not in range.");
        return;
    }
    //setting in the actual hour
    outb(0x70, 0x04);
    outb(0x71, tobcd(atoi(hour)));

    //checking if minute is valid
    char* minute = strtok(NULL, ":");
    if(minute == NULL){
        println("Invalid input, did not put a minute.");
        return;
    }

    int minutes = atoi(minute);
    if(minutes < 0 || minutes > 59){
        println("Invalid input, minute not in range.");
        return;
    }
    //setting the actual minute
    outb(0x70, 0x02);
    outb(0x71, tobcd(atoi(minute)));

    //checking if second is valid
    char* second = strtok(NULL, ":");
    if(second == NULL){
        println("Invalid input, did not put a second.");
        return;
    }

    int seconds = atoi(second);
    if(seconds < 0 || seconds > 59){
        println("Invalid input, second not in range.");
        return;
    }
    //setting the actual seconds
    outb(0x70, 0x00);
    outb(0x71, tobcd(atoi(second)));

    sti();

}

// sets the date
void setdate (int argc, char* argv[]) {
    cli();

    // checking array size and error checking
    if(argc == 1){
        println("Invalid input, did not put the date.");
        return;
    }

    // checking if month is valid
    char* month = strtok(argv[1], "/");
    int months = atoi(month);
    if(months > 12 || months < 1){
        println("Invalid input, month is not in range.");
        return;
    }

    // checking if the day of month is valid
    char* dayofmonth = strtok(NULL, "/");
    if(dayofmonth == NULL) {
        println("Invalid input, did not put a day of the month.");
        return;
    }
    int days = atoi(dayofmonth);
    if(days > 31 || days < 1){
        println("Invalid input, day of month is not in range.");
        return;
    }

    // checking if year is valid
    char* year = strtok(NULL, "/");
    if(year == NULL) {
        println("Invalid input, did not put a year.");
        return;
    }
    //checking length of year
    int years = atoi(year);
    if(years > 99){
        println("Invalid input, year must be two characters long.");
        return;
    }
    // setting the year
    outb(0x70, 0x09);
    outb(0x71, tobcd(atoi(year)));

    // setting the day of month
    outb(0x70, 0x07);
    outb(0x71, tobcd(atoi(dayofmonth)));

    // setting the month
    outb(0x70, 0x08);
    outb(0x71, tobcd(atoi(month)));

    sti();
}

// allows user to get the date
void getdate (int argc, char* argv[]) {
    // Month
    outb(0x70, 0x08);
    printbcd(inb(0x71));
    print("/");

    // Day of month
    outb(0x70, 0x07);
    printbcd(inb(0x71));
    print("/");

    // Year
    outb(0x70, 0x09);
    printbcd(inb(0x71));
    println(" ");
}

// allows user to get the time
void gettime (int argc, char* argv[]) {
    // Hour
    outb(0x70, 0x04);
    printbcd(inb(0x71));
    print(":");

    // Minute
    outb(0x70, 0x02);
    printbcd(inb(0x71));
    print(":");

    // Second
    outb(0x70, 0x00);
    printbcd(inb(0x71));
    println(" ");
}

// function to convert from binary to BCD
int tobcd (int binary) {
    int firstdigit = binary % 10;
    int seconddigit = binary / 10;
    int fulldigit = (seconddigit << 4) + firstdigit;
    return fulldigit;

}

void showPCB(int argc, char* argv[]){
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
void showReady()
{
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

for(i = 0; i< readySuspended->count; i++)
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
    PCB *find = FindPCB(argv[1]);
    if(!find){
        println("PCB not found.");
        return 0;
    }

    RemovePCB(find);
    find -> readyState = -1;
    InsertPCB(find);
    return 1;
}

//Finds a PCB and sets its state to unblocked, then reinserts
int unblockPCB(int argc, char *argv[]){
    PCB *find = FindPCB(argv[1]);
    if(!find){
        println("PCB not found.");
        return 0;
    }

    RemovePCB(find);
    find -> readyState = 1;
    InsertPCB(find);
    return 1;
}

//Finds a PCB and sets its state to suspended, then reinserts
int suspendPCB(int argc, char *argv[]){
    PCB *find = FindPCB(argv[1]);
    if(!find){
        println("PCB not found.");
        return 0;
    }

    RemovePCB(find);
    find -> suspendState = 1;
    InsertPCB(find);
    return 1;
}

//Finds a PCB and sets its state to resumed, then reinserts
int resumePCB(int argc, char *argv[]){
    PCB *find = FindPCB(argv[1]);
    if(!find){
        println("PCB not found.");
        return 0;
    }

    RemovePCB(find);
    find -> suspendState = 0;
    InsertPCB(find);
    return 1;
}

//Finds PCB, sets priority, reinserts
int setPriority(int argc, char *argv[]){
    PCB *find = FindPCB(argv[1]);
    if(!find){
        println("PCB not found.");
        return 0;
    }

    int p = atoi(argv[2]);
    if(p < 0 || p > 9) return 0;

    RemovePCB(find);
    find -> priority = p;
    InsertPCB(find);
    return 1;
}

//int frombcd (int bcd){
//    int firstdigit = bcd >> 4;
//    int seconddigit = bcd & 15;
//    int fulldigit = (firstdigit * 10) + seconddigit;
//    return fulldigit;
//}

// function to print out BCD
void printbcd (int bcd){
    int firstdigit = bcd >> 4;
    int seconddigit = bcd & 15;
    char ascii = firstdigit + 48;
    char asciii = seconddigit + 48;
    char array[3] = {ascii , asciii, '\0'};
    print(array);
}

//Creates PCB
void CreatePCB(int argc, char *argv[]){
    //Checks if argc is the appropriate value
    if(argc != 4){
        println("Error, not in range.");
        return;
    }
    //Sets parameters to argv
    char* name = argv[1];
    int class = atoi(argv[2]);
    int priority = atoi(argv[3]);

    //Error checking the name length, priority, and class
    if(FindPCB(name) != NULL){
        println("Error, name must be unique and valid.");
        return;
    }

    if(strlen(name) < 8){
        println("Error, name has to be AT LEAST 8 characters.");
        return;
    }

    if(class != 0 && class != 1){
        println("Error. Class must be between 0 and 1.");
        return;
    }

    if(priority < 0 || priority > 9){
        println("Error. Priority must be between 0 and 9.");
        return;
    }
    //Inserts PCB
    PCB* pcb = SetupPCB(name, class, priority);
    InsertPCB(pcb);
}

//Deletes the PCB
void DeletePCB(int argc, char *argv[]){
    //Checks if argc is the appropriate value
    if(argc != 2){
        println("Error, not in range.");
        return;
    }
    //Sets parameter to argv
    char* name = argv[1];

    //Error checks name
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