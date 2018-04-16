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
        println("The commands you can use include: clear, shutdown, setdate, getdate, settime, gettime, version, showPCB, showReady, showBlocked, showAll, createPCB, deletePCB, block, unblock, suspend, resume, and setPriority.");
    }

    // Display the help message for the command they specified
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
            println("The createPCB function creates a PCB with the user input name, class, and priority. The createPCB command can be used by typing createPCB.");
        }

        else if (strcmp(argv[1], "deletePCB") == 0) {
            println("The deletePCB function deletes a PCB with the user input name. The deletePCB command can be used by typing deletePCB.");
        }

        else if (strcmp(argv[1], "block") == 0){
            println("The block command can accept one argument, that being the name of a process. Block will find a process and change its state to blocked.");
        }

        else if (strcmp(argv[1], "unblock") == 0){
            println("The unblock command can accept one argument, that being the name of a process. Unblock will find a process and change its state to unblocked.");
        }

        else if (strcmp(argv[1], "suspend") == 0){
            println("The suspend command can accept one argument, that being the name of a process. Suspend will find a process and change its state to suspended.");
        }

        else if (strcmp(argv[1], "resume") == 0){
            println("The resume command can accept one argument, that being the name of a process. Resume will find a process and change its state to resumed.");
        }

        else if (strcmp(argv[1], "setPriority") == 0){
            println("The setPriority command can accept two arguments. The name of a process, and a priority to set in the process. The priority can be an integer between 0 and 9. SetPriority will find the process and then set the priority.");
        }

        else if (strcmp(argv[1], "yield") == 0){
            println("This a temporary command that was removed as of R4. It would call sys_req(IDLE) to allow comhand to let other processes run if there were any in the queue. Usage 'yield'");
        }

        else if (strcmp(argv[1], "loadr3") == 0){
            println("This a temporary command that was removed as of R4. It would create 5 processes and add them to the ready queue. Usage: 'loadr3'");
        }

        else if (strcmp(argv[1], "isEmpty") == 0){
            println("The isEmpty command takes in no arguments. It displays true if there is no allocated memory, false otherwise");
        }

        else if (strcmp(argv[1], "showAllocatedMemory") == 0){
            println("The showAllocatedMemory command takes in no arguments. It displays the starting address and size of all the blocks of allocated memory.");
        }

        else if (strcmp(argv[1], "showFreeMemory") == 0){
            println("The showFreeMemory command takes in no arguments. It displays the starting address and size of all the blocks of free memory");
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
    println("Version: Final");
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