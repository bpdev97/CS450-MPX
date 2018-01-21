
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
    outb(0x71, atoi(hour); //TODO convert to BCD

    char* minute = strtok(NULL, ":");
    outb(0x70, 0x02);
    outb(0x71, atoi(minute)); //TODO convert to BCD

    char* second = strtok(NULL, ":");
    outb(0x70, 0x00);
    outb(0x71, atoi(second)); //TODO convert to BCD

    sti();
}

void setdate (int argc, char* argv[]) {
    cli();

    char* dayofmonth = strtok(argv[1], "/");
    outb(0x70, 0x07);
    outb(0x71, atoi(dayofmonth)); //TODO convert to BCD

    char* month = strtok(NULL, "/");
    outb(0x70, 0x08);
    outb(0x71, atoi(month)); //TODO convert to BCD

    char* year = strtok(NULL, "/");
    outb(0x70, 0x09);
    outb(0x71, atoi(year)); //TODO convert to BCD

    sti();
}

void getdate (int argc, char* argv[]) {
    char* dayofmonth = argv[1];
    outb(0x70, 0x07);
    inb(0x71);
}

void gettime (int argc, char* argv[]) {
    char *hour = argv[1];
    outb(0x70, 0x04);
    inb(0x71);
}