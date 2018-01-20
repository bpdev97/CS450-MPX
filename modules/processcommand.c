
#include <string.h>
#include <core/serial.h>
#include "processcommand.h"

void processcommand (int numword, char* args[]){

    char* command = args[0];
    if(strcmp(command, "help") == 0){
        serial_println("congratulations");
    }
    else{
        serial_println("incorrect");
    }


}