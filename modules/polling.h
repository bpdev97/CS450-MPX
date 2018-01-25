#ifndef _POLLING_H
#define _POLLING_H

#include <system.h>

#define ENTER 13
#define BACKSPACE 127

/*
  Procedure..: poll
  Description..: poll from character register to get inpit
  Params..: buffer pointer and count pointer
*/
int poll(char * buffer, int* count);
#endif
