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
  char *argv[20]; // Maximum number of command arguments is 19

  while(!quit) {
    print("~> "); // print prompt
    memset(cmdBuffer, '\0', 100); // make cmdBuffer all terminators
    bufferSize = 99; // reset size before each call to read 
    sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize); // poll for user input

    // Build argv and argc
    int argc = 0;
    char *token = strtok(cmdBuffer, " ");
    while(token) {
      argv[argc++] = token;
      token = strtok(NULL, " ");
    }
    
    // Process and run the recieved command
    if(strcmp(argv[0], "help") == 0) {
      help(argc, argv);
    }

    else if(strcmp(argv[0], "version") == 0) {
      version(argc, argv);
    }

    // shutdown the os
    else if(strcmp(argv[0], "shutdown") == 0 || strcmp(argv[0], "quit") == 0
      || strcmp(argv[0], "exit") == 0) {
      println("Are you sure you want to shutdown? Enter Y or N.");
      int num = 1;
      char array[1];
      poll(array, &num);
      strcmp(array, "Y");
      strcmp(array, "N");

      if(strcmp(array, "Y") == 0) {
        quit = 1;
      }
    }

    // Clear the screen
    else if(strcmp(argv[0], "clear") == 0) {
      print(" \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      print(" \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      print(" \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    }

    else if(strcmp(argv[0], "settime") == 0) {
      settime(argc, argv);
    }

    else if(strcmp(argv[0], "gettime") == 0) {
      gettime(argc, argv);
    }
    
    else if(strcmp(argv[0], "setdate") == 0) {
      setdate(argc, argv);
    }

    else if(strcmp(argv[0], "getdate") == 0) {
      getdate(argc, argv);
    }
   
    else if(strcmp(argv[0], "showPCB") == 0) {
      showPCB(argc, argv);
    }
  
    else if(strcmp(argv[0], "showReady") == 0) {
      showReady();
    }

    else if(strcmp(argv[0], "showBlocked") == 0) {
      showBlocked();
    }

    else if(strcmp(argv[0], "showAll") == 0) {
      showAll();
    }
    else if(strcmp(argv[0], "CreatePCB") == 0){
      CreatePCB(argc, argv);

    }

    
    // Command not recognized
    else {
      print("Command not found: ");
      println(argv[0]);
    }
  }
  return 0; // quit was entered
}
