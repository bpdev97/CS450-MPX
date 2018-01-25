#include <system.h>
#include <core/serial.h>
#include <core/io.h>
#include <string.h>
#include "polling.h"
#include "mpx_supt.h"

/*
  Procedure..: poll
  Description..: 
  Params..: 
*/

int poll(char * buffer, int* count) {
	char newBuffer[100]; //size will be alocated later
	char letter[4];
	int counter = 0;
	int maxBufferSize = *count;
	int exit =0;
	int enter = 0;
	char emptyChar = ' ';
	char backspace[] = {'\b',' ','\b','\0'};

	while(!enter) {  
    	if (inb(COM1+5)&1) { // Check for character
            letter[0] = inb(COM1); // Build letter from inb(COM1)
            letter[1] = inb(COM1);
            letter[2] = inb(COM1);
            letter[3] = '\0';

			// Look for special case characters
			if ( (letter[0] != BACKSPACE) && (letter[0] != ENTER) && (letter[0] != '\033') ) {
				newBuffer[++counter] = letter[0]; // Add character to buffer and then increment counter
				// LOOK AT THIS LATER SOMETHING ISNT RIGHT... BUFFER NEEDS TO BE INIT TO NULL TERM
				// This will be done in the command handler and when we pass the buffer in
				newBuffer[++counter] = '\0'; // adds null terminate
				
				int count = 4;
				sys_req(WRITE, COM1, letter, &count); // Write the character to COM1
			}

			// Handle special case characters
			else {
				// Enter Recieved
				if(strcmp(letter, "\r\000\000") == 0) {
					int i = 0;
					serial_println("");

					for(i = 0; i < counter; i++) {
						sys_req(WRITE, COM1, &newBuffer[i], &counter);
					}
					enter = 1;                                 
				}
					
				// Backspace Recieved
				else if(strcmp(letter, "\177\000\000") == 0) {			
					newBuffer[counter--] = emptyChar; // @Joe can you explain why we have to do this twice
					newBuffer[counter--] = emptyChar; // I'm a bit confused on that
					int backCount = 4;
					sys_req(WRITE, COM1, &backspace, &backCount);
				}
				
				// Delete recieved
				else if (strcmp(letter, "\033[3") == 0) {
					serial_print("\nDelete recieved!");
					int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
				}

				// Left arrow Key recieved
				else if (strcmp(letter, "\033[D") == 0) {
					int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
				}

				// Right arrow Key recieved
				else if (strcmp(letter, "\033[C") == 0) {
					serial_print("\nRight arrow recieved!");
					int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
					// counter++; // Move buffer counter to the right
				}
			}
		}
	}
	
	// Enter pressed
	count = &counter;   
	buffer = newBuffer;
	return 0;
}