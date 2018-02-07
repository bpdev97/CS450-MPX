
#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include "mpx_supt.h"
#include "processcommand.h"
#include "pcb.h"
#include "queue.h"

void help (int argc, char* argv[]) {

    //checking array size and setting up the Help menu
    if (argc == 1) {
        println("To use the help command type 'help' followed by the command you would like to use.");
        println("The commands you can use include: clear, shutdown, setdate, getdate, settime, gettime, and version.");
    }
    //listing each command and what they do
    else if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            println("The shutdown command shuts down the computer system.");
        } 
        
        else if (strcmp(argv[1], "clear") == 0) {
            println("The clear command clears com1 by printing a number of new lines.");
        }        

        else if (strcmp(argv[1], "setdate") == 0) {
            println("The Set date command allows the user to input a date.");
        } 
        
        else if (strcmp(argv[1], "getdate") == 0) {
            println("The getdate command allows the user to retrieve the set date.");
        } 
        
        else if (strcmp(argv[1], "settime") == 0) {
            println("The settime command allows the user to input a time.");
        } 
        
        else if (strcmp(argv[1], "gettime") == 0) {
            println("The gettime command allows the user to retrieve the set time.");
        } 
        
        else if (strcmp(argv[1], "version") == 0) {
            println("The version command displays the current version of MPX and the completion date.");
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
    println("Version: R1.");
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
/*
//display PCB info using the name from user
void showPCB(char * name){
  pcb* selectedPCB = findPCB(name);
  if(selectedPCB == null){
    println("PCB name not found");
  }
  else{
    println("Process Name: " + seletedPCB->name);
    println("Class: " + seletedPCB->classNum); //need int to string
    if(seletedPCB->readyState == 1){
      println("State: Ready");  
    }
    else{
      println("State: Blocked");  
    }
   if(seletedPCB->suspendState == 1){
      println("Suspended Status: Suspended ");
    }
    else{
      println("Suspended Status: Not Suspended");
    }
    println("Priority: " + seletedPCB->priority);// need int to string
  }
  
}
//display all PCB info in ready queues
void showReady(){
 readyQueue testQueue;
 suspendedReadyQueue testQueue2;

 int i;
 for(i = 0; i< testQueue.count; i++)
 {
   pcb* currentPCB = (testQueue.tail + i); //need to find out how to increment through queue
   println("Process Name: " + currentPCB->name);
   println("Class: " + currentPCB->classNum); //need int to string
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
   println("Priority: " + currentPCB->priority);// need int to string
 }

 for(i = 0; i< testQueue2.count; i++)
 {
   pcb* currentPCB = (testQueue2.tail + i); //need to find out how to increment through queue
   println("Process Name: " + currentPCB->name);
   println("Class: " + currentPCB->classNum); //need int to string
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
   println("Priority: " + currentPCB->priority);// need int to string
 }
}
//display all PCB info in blocked queues
void showBlocked(){
 blockedQueue testQueue;
 suspendedBlockedQueue testQueue2;

 int i;
 for(i = 0; i< testQueue.count; i++)
 {
   pcb* currentPCB = (testQueue.tail + i); //need to find out how to increment through queue
   println("Process Name: " + currentPCB->name);
   println("Class: " + currentPCB->classNum); //need int to string
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
   println("Priority: " + currentPCB->priority);// need int to string
 }

 for(i = 0; i< testQueue2.count; i++)
 {
   pcb* currentPCB = (testQueue2.tail + i); //need to find out how to increment through queue
   println("Process Name: " + currentPCB->name);
   println("Class: " + currentPCB->classNum); //need int to string
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
   println("Priority: " + currentPCB->priority);// need int to string
 }
}
//displays all PCB info
void showAll(){
  showReady();
  showBlocked();
}

*/
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
