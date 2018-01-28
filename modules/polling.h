#ifndef _POLLING_H
#define _POLLING_H

#include <system.h>

#define ENTER 13
#define BACKSPACE 127
#define TAB 9
/*
  Procedure..: poll
  Description..: poll from character register to get inpit
  Params..: buffer pointer and count pointer
*/
int poll(char * buffer, int* count);
void clearCommandLine(int counter, int cursorPosition);
void resetCursor(int cursorPosition);
#endif
