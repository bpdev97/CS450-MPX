#include <system.h>
#include <core/serial.h>
#include "comhand.h"

/*
  Procedure..: comhand
  Description..: Process/execute user input gathered from polling 
  Params..: void
*/

int comhand() {
  // TESTING MAKEFILE STUFF
  serial_print("\n################# TESTING MAKEFILE CHANGES ################# \n");
  
//  char cmdBuffer[100];
//  int bufferSize;
//  int quit=0;
//  
//  while(!quit) {
//    // get a command
//    memset(buffer,‘\0’, 100);
//    bufferSize = 99; // reset size before each call to read 
//    sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
//  }
  return 0; // quit was entered
}
