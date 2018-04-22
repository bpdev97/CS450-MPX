#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include <system.h>
#include "mpx_supt.h"
#include "r1.h"

// Displays a help messaage with usage for the specified command to the user
void help (int argc, char* argv[]) {
    // Display help message for help itself
    if (argc == 1) {
        println("To use the help command type 'help' followed by the command you would like to use.");
        println("The commands you can use include: clear, shutdown, setdate, getdate, settime, gettime, version, showPCB, showReadyPCBs, showBlockedPCBs, showAllPCBs, blockPCB, unblockPCB, suspendPCB, resumePCB, and setPCBPriority.");
    }

    // Display the help message for the command they specified
    else if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            println("The shutdown command shuts down the operating system. Shutdown takes no arguments and will prompt the user to enter 'Y' or 'N' to choose whether to really shutdown or not.");
            println("Usage: shutdown");
        } 
        
        else if (strcmp(argv[1], "clear") == 0) {
            println("The clear command effectively clears the terminal display. The clear command prints out new line characters to move past outputs off the screen, simulating clearing the screen.");
            println("Usage: clear");
        }        

        else if (strcmp(argv[1], "setdate") == 0) {
            println("The setdate command allows the user to input and set the system date.");
            println("Usage: setdate MM/DD/YY");
        } 
        
        else if (strcmp(argv[1], "getdate") == 0) {
            println("The getdate command will print out the current date.");
            println("Usage: getdate");
        } 
        
        else if (strcmp(argv[1], "settime") == 0) {
            println("The settime command allows the user to input and set the system time. Settime only works with 24 hour time and the hour, minute, and second must all be entered for the input to be valid.");
            println("Usage: settime HH:MM:SS");
        } 
        
        else if (strcmp(argv[1], "gettime") == 0) {
            println("The gettime command will retrieve and print out the current system time in 24 hour format.");
            println("Usage: gettime");
        } 
        
        else if (strcmp(argv[1], "version") == 0) {
            println("The version command displays the current version of MPX and the completion date.");
            println("Usage: version");
        }

        else if (strcmp(argv[1], "showPCB") == 0) {
            println("The showPCB checks to see if the PCB name exists in the in the ready queue, readySuspended queue, blocked queue, or blockedSuspended queue. If the PCB exists then the name, class, ready state, blocked state, and priority of the PCB is displayed.");
            println("Usage: showPCB pcbName");
        }

        else if (strcmp(argv[1], "showReadyPCBs") == 0) {
            println("The showReadyPCBs command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue and readySuspended queue.");
            println("Usage: showReadyPCBs");
        }

        else if (strcmp(argv[1], "showBlockedPCBs") == 0) {
            println("The showBlockedPCBs command displays the name, class, ready state, blocked state, and priority for all PCBs in the blocked queue and blockedSuspended queue.");
            println("Usage: showBlockedPCBs");
        }

        else if (strcmp(argv[1], "showAllPCBs") == 0) {
            println("The showAllPCBs command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue, readySuspended queue, blocked queue, and blockedSuspended queue.");
            println("Usage: showAllPCB");
        }

        else if (strcmp(argv[1], "blockPCB") == 0){
            println("blockPCB will change a PCB's state to blocked.");
            println("Usage: blockPCB pcbName");
        }

        else if (strcmp(argv[1], "unblockPCB") == 0){
            println("The unblock command will unblock a blocked PCB.");
            println("Usage: unblockPCB pcbName");
        }

        else if (strcmp(argv[1], "suspendPCB") == 0){
            println("The suspend command will change a PCB's state to suspended.");
            println("Usage: suspendPCB pcbName");
        }

        else if (strcmp(argv[1], "resumePCB") == 0){
            println("The resume command will find a PCB and change its state to resumed.");
            println("Usage: resumePCB pcbName");
        }

        else if (strcmp(argv[1], "setPCBPriority") == 0){
            println("The setPCBPriority command can accept two arguments. The name of a process, and a priority to set in the process. The priority can be an integer between 0 and 9. SetPriority will find the process and then set the priority.");
            println("Usage: setPCBPriority pcbName priority");
        }

        else if (strcmp(argv[1], "isMemoryEmpty") == 0){
            println("The isMemoryEmpty command displays true if there is no allocated memory, false otherwise");
            println("Usage: isMemoryEmpty");
        }

        else if (strcmp(argv[1], "showAllocatedMemory") == 0){
            println("The showAllocatedMemory command displays the starting address and size of all the blocks of allocated memory.");
            println("Usage: showAllocatedMemory");
        }

        else if (strcmp(argv[1], "showFreeMemory") == 0){
            println("The showFreeMemory command displays the starting address and size of all the blocks of free memory");
            println("Usage: showFreeMemory");
        }

        // Error handling
        else{
            println("Invalid input, command not found.");
        }
    }

    // Wrong number of arguments specified
    else{
        println("Invalid input, too many arguments.");
    }
}

// Displays the current version of the MPX project
void version () {
    println("Version: Release 1.0");
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
void getdate () {
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
void gettime () {
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

// function to print out BCD
void printbcd (int bcd){
    int firstdigit = bcd >> 4;
    int seconddigit = bcd & 15;
    char ascii = firstdigit + 48;
    char asciii = seconddigit + 48;
    char array[3] = {ascii , asciii, '\0'};
    print(array);
}