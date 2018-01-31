#include <system.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include "processcommand.h"
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
  char *argv[20];

  // char tempstring[50];
  // sprintf(tempstring, "Hello World");
  // println(tempstring);
  
  while(!quit) {
    sys_req(WRITE, DEFAULT_DEVICE, "~> ", 3); // print prompt
    memset(cmdBuffer, '\0', 100); // make cmdBuffer all terminators
    bufferSize = 99; // reset size before each call to read 
    sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);

    // Build argv and argc
    int argc = 0;
    char *token = strtok(cmdBuffer, " ");
    while(token) {
      argv[argc++] = token;
      //memset(argv[argc], '\0', 20);
      //strcpy(argv[argc++], token);
      //serial_println(argv[argc - 1]);
      token = strtok(NULL, " ");
    }
    

    // Run the correct command
    if(strcmp(argv[0], "help") == 0) {
      help(argc, argv);
    }

    else if(strcmp(argv[0], "version") == 0) {
      version(argc, argc);
    }

    else if(strcmp(argv[0], "shutdown") == 0 || strcmp(argv[0], "quit") == 0) {
      quit = 1;
    }

    else if(strcmp(argv[0], "settime") == 0) {
      settime(argc, argc);
    }

    else if(strcmp(argv[0], "gettime") == 0) {
      gettime(argc, argc);
    }
    
    else if(strcmp(argv[0], "setdate") == 0) {
      setdate(argc, argc);
    }

    else if(strcmp(argv[0], "getdate") == 0) {
      getdate(argc, argc);
    }
  }
  return 0; // quit was entered
}
