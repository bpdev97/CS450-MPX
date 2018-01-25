
#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include "processcommand.h"
#include "println.h"

void help (int argc, char* argv[]) {

    //checking array size and setting up the Help menu
    if (argc == 1) {
        print("To use the help command type 'help' followed by the command you would like to use.");
        print("The commands you can use include: Help, Shutdown, and Date and Time.");
    }
    //listing each command and what they do
    else if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            print("The Shutdown command shuts down the computer system.");
        } else if (strcmp(argv[1], "setdate") == 0) {
            print("The Set Date command allows the user to input a date.");
        } else if (strcmp(argv[1], "getdate") == 0) {
            print("The Get Date command allows the user to retrieve the set date.");
        } else if (strcmp(argv[1], "settime") == 0) {
            print("The Set Time command allows the user to input a time.");
        } else if (strcmp(argv[1], "gettime") == 0) {
            print("The Get Time command allows the user to retrieve the set time.");
        }
     //error checking
        else{
            print("Invalid input, command not valid.");
        }
    }
    else{
        print("Invalid input, too many arguments.");
    }
}

//shuts down system
void shutdown (int argc, char* argv[]) {
    print("Are you sure you want to shut down?");
}

//allows user to set the time
void settime (int argc, char* argv[]) {
    cli();

    //checking array size and error checking
    if(argc == 1){
        print("Invalid input, did not put the time.");
        return;
    }

    //checking if hour is valid
    char* hour = strtok(argv[1], ":");
    int hours = atoi(hour);
    if(hours < 0 || hours > 24){
        print("Invalid input, hour not in range.");
        return;
    }
    //setting in the actual hour
    outb(0x70, 0x04);
    outb(0x71, tobcd(atoi(hour)));

    //checking if minute is valid
    char* minute = strtok(NULL, ":");
    if(minute == NULL){
        print("Invalid input, did not put a minute.");
        return;
    }

    int minutes = atoi(minute);
    if(minutes < 0 || minutes > 59){
        print("Invalid input, minute not in range.");
        return;
    }
    //setting the actual minute
    outb(0x70, 0x02);
    outb(0x71, tobcd(atoi(minute)));

    //checking if second is valid
    char* second = strtok(NULL, ":");
    if(second == NULL){
        print("Invalid input, did not put a second.");
        return;
    }

    int seconds = atoi(second);
    if(seconds < 0 || seconds > 59){
        print("Invalid input, minute not in range.");
        return;
    }
    //setting the actual seconds
    outb(0x70, 0x00);
    outb(0x71, tobcd(atoi(second)));

    sti();

}

//allows user to set the date
void setdate (int argc, char* argv[]) {
    cli();

    //checking array size and error checking
    if(argc == 1){
        print("Invalid input, did not put the date.");
        return;
    }

    //checking if month is valid
    char* month = strtok(argv[1], "/");
    int months = atoi(month);
    if(months > 12 || months < 1){
        print("Invalid input, month is not in range.");
        return;
    }
    //setting the month
    outb(0x70, 0x08);
    outb(0x71, tobcd(atoi(month)));

    //checking if the day of month is valid
    char* dayofmonth = strtok(NULL, "/");
    if(dayofmonth == NULL) {
        print("Invalid input, did not put a day of the month.");
        return;
    }
    int days = atoi(dayofmonth);
    if(days > 31 || days < 1){
        print("Invalid input, day of month is not in range.");
        return;
    }
    //setting the day of month
    outb(0x70, 0x07);
    outb(0x71, tobcd(atoi(dayofmonth)));

    //checking if year is valid
    char* year = strtok(NULL, "/");
    if(year == NULL) {
        print("Invalid input, did not put a year.");
        return;
    }
    //setting the year
    outb(0x70, 0x09);
    outb(0x71, tobcd(atoi(year)));

    sti();
}

//allows user to get the date
void getdate (int argc, char* argv[]) {
    //Month
    outb(0x70, 0x08);
    printbcd(inb(0x71));
    serial_print("/");

    //Day of month
    outb(0x70, 0x07);
    printbcd(inb(0x71));
    serial_print("/");

    //Year
    outb(0x70, 0x09);
    printbcd(inb(0x71));
    print(" ");
}

//allows user to get the time
void gettime (int argc, char* argv[]) {
    //Hour
    outb(0x70, 0x04);
    printbcd(inb(0x71));
    serial_print(":");

    //Minute
    outb(0x70, 0x02);
    printbcd(inb(0x71));
    serial_print(":");

    //Second
    outb(0x70, 0x00);
    printbcd(inb(0x71));
    print(" ");
}

//function to convert from binary to BCD
int tobcd (int binary) {
    int firstdigit = binary % 10;
    int seconddigit = binary / 10;
    int fulldigit = (seconddigit << 4) + firstdigit;
    return fulldigit;

}

//int frombcd (int bcd){
//    int firstdigit = bcd >> 4;
//    int seconddigit = bcd & 15;
//    int fulldigit = (firstdigit * 10) + seconddigit;
//    return fulldigit;
//}

//function to print out BCD
void printbcd (int bcd){
    int firstdigit = bcd >> 4;
    int seconddigit = bcd & 15;
    char ascii = firstdigit + 48;
    char asciii = seconddigit + 48;
    char array[3] = {ascii , asciii, '\0'};
    serial_print(array);
}