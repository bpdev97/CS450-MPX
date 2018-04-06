#include <system.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"
#include "processcommand.h"
#include "procsr3.h"
#include "pcb.h"
#include "comhand.h"
#include "mcb.h"

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
      version();
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
        // MURDER!!!!! the idle process
        RemovePCB(FindPCB("idle"));
        // Kill the comhand process
        sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
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
      gettime();
    }
    
    else if(strcmp(argv[0], "setdate") == 0) {
      setdate(argc, argv);
    }

    else if(strcmp(argv[0], "getdate") == 0) {
      getdate();
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

    else if(strcmp(argv[0], "createPCB") == 0){
      //CreatePCB(argc, argv);
    }

    else if(strcmp(argv[0], "deletePCB") == 0){
      DeletePCB(argc, argv);
    }

    else if(strcmp(argv[0], "block") == 0) {
      blockPCB(argc, argv);
    }

    else if(strcmp(argv[0], "unblock") == 0) {
      unblockPCB(argc, argv);
    }

    else if(strcmp(argv[0], "suspend") == 0) {
      suspendPCB(argc, argv);
    }

    else if(strcmp(argv[0], "resume") == 0) {
      resumePCB(argc, argv);
    }

    else if(strcmp(argv[0], "setPriority") == 0) {
      setPriority(argc, argv);
    }

    else if(strcmp(argv[0], "yield") == 0) {
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }

    else if(strcmp(argv[0], "loadr3") == 0) {
      InsertPCB(SetupPCB("proc1", 1, 1, &proc1));
      InsertPCB(SetupPCB("proc2", 1, 1, &proc2));
      InsertPCB(SetupPCB("proc3", 1, 1, &proc3));
      InsertPCB(SetupPCB("proc4", 1, 1, &proc4));
      InsertPCB(SetupPCB("proc5", 1, 1, &proc5));
    }

    else if(strcmp(argv[0], "isEmpty") == 0) {
      isEmpty();
    }

    else if(strcmp(argv[0], "showAllocatedMemory") == 0) {
      showAllocatedMemory();
    }

    else if(strcmp(argv[0], "showFreeMemory") == 0) {
      showFreeMemory();
    }

    else if(strcmp(argv[0], "initializeHeap") == 0) {
      initializeHeap(2000);
    }

    else if(strcmp(argv[0], "allocateMemory") == 0) {
      allocateMemory(atoi(argv[1]));
    }

    else if(strcmp(argv[0], "freeMemory") == 0) {
      freeMemory((int) atoi(argv[1]));
    }

    else if(strcmp(argv[0], "testR5") == 0) {
      println("Initializing the Heap...");
      initializeHeap(30000);
      showFreeMemory();
      println("Allocating Memory blocks...");
      int i = 0;
      for (; i < 5; i++){
        allocMem(1024 + 8*i);
      }
      showAllocatedMemory();
      println("Freeing 3 Memory blocks...");
      i = 0;
      for (; i < 3; i++){
        freeMem(AMCB -> beginning);
      }
      showFreeMemory();
      showAllocatedMemory();
    }

    // Command not recognized
    else {
      print("Command not found: ");
      println(argv[0]);
    }
  }
  return 0; // quit was entered
}
