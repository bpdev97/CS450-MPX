
#include <string.h>
#include <core/serial.h>
#include <core/io.h>
#include "processcommand.h"

void help (int argc, char* argv[]) {

    //checking number of user inputs
    if (argc == 1) {
        serial_println("To use the help command type 'help' followed by the command you would like to use.");
        serial_println("The commands you can use include: Help, Shutdown, and Date and Time.");
    }
    else if (argc == 2) {
        if (strcmp(argv[1], "shutdown") == 0) {
            serial_println("The Shutdown command shuts down the computer system.");
        } else if (strcmp(argv[1], "setdate") == 0) {
            serial_println("set date "); //TODO
        } else if (strcmp(argv[1], "getdate") == 0) {
            serial_println("get date "); //TODO
        } else if (strcmp(argv[1], "settime") == 0) {
            serial_println("set time "); //TODO
        } else if (strcmp(argv[1], "gettime") == 0) {
            serial_println("get time "); //TODO
        }
        else{
            serial_println("Invalid input, command not valid.");
        }
    }
    else{
        serial_println("Invalid input, too many arguments");
    }
}

void shutdown (int argc, char* argv[]) {
    serial_println("Are you sure you want to shut down?");
}

void settime (int argc, char* argv[]) {
    cli();

    char* hour = strtok(argv[1], ":");
    outb(0x70, 0x04);
    outb(0x71, tobcd(atoi(hour)));

    char* minute = strtok(NULL, ":");
    outb(0x70, 0x02);
    outb(0x71, tobcd(atoi(minute)));

    char* second = strtok(NULL, ":");
    outb(0x70, 0x00);
    outb(0x71, tobcd(atoi(second)));

    sti();
}

void setdate (int argc, char* argv[]) {
    cli();

    char* month = strtok(argv[1], "/");
    outb(0x70, 0x08);
    outb(0x71, tobcd(atoi(month)));

    char* dayofmonth = strtok(NULL, "/");
    outb(0x70, 0x07);
    outb(0x71, tobcd(atoi(dayofmonth)));

    char* year = strtok(NULL, "/");
    outb(0x70, 0x09);
    outb(0x71, tobcd(atoi(year)));

    sti();
}

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
    serial_println(" ");
}

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
    serial_println(" ");
}

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

void printbcd (int bcd){
    int firstdigit = bcd >> 4;
    int seconddigit = bcd & 15;
    char ascii = firstdigit + 48;
    char asciii = seconddigit + 48;
    char array[3] = {ascii , asciii, '\0'};
    serial_print(array);
}
