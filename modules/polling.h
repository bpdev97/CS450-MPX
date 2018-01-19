#ifndef _POLLING_H
#define _POLLING_H

#include <system.h>

#define EXIT 0
#define IDLE 1
#define READ 2
#define WRITE 3
#define INVALID_OPERATION 4

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

/*
  Procedure..: poll
  Description..: poll from character register to get inpit
  Params..: buffer pointer and count pointer
*/
int poll(char * buffer, int* count);
int checkCommand(char * buffer, char * command, int counter);
#endif
