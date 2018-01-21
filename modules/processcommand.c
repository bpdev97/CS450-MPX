
#include <string.h>
#include <core/serial.h>
#include "processcommand.h"

void help (int argc, char* argv[]) {

    if (argc == 1) {
        serial_println("To use the help command type 'help' followed by the command you would like to use.");
        serial_println("The commands you can use include: Help, Shutdown, and Date and Time.");
    } else if (argc == 2) {
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