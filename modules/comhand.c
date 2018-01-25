#include <system.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include "comhand.h"

/*
  Procedure..: comhand
  Description..: Process/execute user input gathered from polling 
  Params..: void
*/

int comhand() {
  // setup polling function
  int (*polling) (char * buffer, int* count);
  polling = &poll;
  sys_set_read(polling);
  char cmdBuffer[100];
  int bufferSize;
  int quit=0;
  
  while(!quit) {
    memset(cmdBuffer, '\0', 100); // make cmdBuffer all terminators
    bufferSize = 99; // reset size before each call to read 
    sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
    sys_req(WRITE, COM1, cmdBuffer, &bufferSize);

  }
  return 0; // quit was entered
}