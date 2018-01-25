//
// Created by Yasmine Pourarian on 1/25/18.
//

#include <core/serial.h>
#include <string.h>
#include "println.h"
#include "mpx_supt.h"

void print (char* string){

    int character = strlen(string);

    sys_req(WRITE, COM1, string, & character);

}


