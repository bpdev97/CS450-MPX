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
        int leftCount = 0;
	int rightCount = 0;

	while(!enter) {  
    	if (inb(COM1+5)&1) { // Check for character
            letter[0] = inb(COM1); // Build letter from inb(COM1)
            letter[1] = inb(COM1);
            letter[2] = inb(COM1);
            letter[3] = '\0';

			// Look for special case characters
			if ( (letter[0] != BACKSPACE) && (letter[0] != ENTER) && (letter[0] != '\033') &&  (letter[0] != '~') )
			 {
				if((leftCount-rightCount) == 0)
				{
				   newBuffer[++counter] = letter[0]; // Add character to buffer and then increment counter
				   // LOOK AT THIS LATER SOMETHING ISNT RIGHT... BUFFER NEEDS TO BE INIT TO NULL TERM
				   // This will be done in the command handler and when we pass the buffer in
				   newBuffer[++counter] = '\0'; // adds null terminate
				
				   int count = 4;
				   sys_req(WRITE, COM1, letter, &count); // Write the character to COM1
				}

				else if((leftCount - rightCount) > 0)//Cursor is moved to the left
				{
				   int counterSaver = counter;// this was added because the algorithm changes counter to something unexpected, not sure why but this is a temp fix
 		 		   counter -= ((leftCount - rightCount)); //moves pointer to null terminator left of char to be inserted
		 		   char temp1 =  newBuffer[++counter]; //saves character being overwritten by new character
		 		   char temp2; 			    //saves the next character in the buffer
		 		   newBuffer[counter] = letter[0]; // writes new letter overtop of the char saved in temp1 
		 		   int i;
				  
				   for(i = 0; i<(leftCount - rightCount); i+=2)
		                   {
		    			 counter += 2; // moves ptr over to next character
		    			 temp2 = newBuffer[counter]; //saves next character in sequence
		    			 newBuffer[counter] = temp1; // replaces character with char saved in temp 1
		     			 temp1 = temp2; // temp 1 equal temp 2
		  		   }
			          newBuffer[++counter] = temp1;//adds last character
				  newBuffer[++counter] = '\0';
				  
  				  counter = counterSaver +2; //sets counter to what counter should be 
			          int count = 4;
				  sys_req(WRITE, COM1, letter, &count); // Write the character to COM1
		  		  
				}
				
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
					//serial_print("\nDelete recieved!");

					//this caused a lot of problems in memory that I can't explain
					/*int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1*/
					
					//works for the most part except it adds an extra space at the end of the buffer and im not sure why
					if((leftCount - rightCount) > 0)
					{
					
			                 counter -= ((leftCount - rightCount)-1); //moves pointer to  char that is to be deleted
		 		   	 int i;
				  	 for(i = 0; i<((leftCount - rightCount)); i+=2)
		                  	 {
						newBuffer[counter] = newBuffer[counter+2];
		    			 	counter+=2;
		  		   	 }
					counter-=2;//move back 2 to delete last character
			          	newBuffer[counter] = emptyChar;//clears last character
				        newBuffer[counter++] = emptyChar;//clears last null terminator
				        
					}

					
				}

				// Left arrow Key recieved
				else if (strcmp(letter, "\033[D") == 0) {
					int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
					if((leftCount-rightCount) < counter)//will not go before the buffer starts
					{
			   		   leftCount +=2;
 					}
				}

				// Right arrow Key recieved
				else if (strcmp(letter, "\033[C") == 0) {
					//serial_print("\nRight arrow recieved!");
					int count = 4;
					sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
					if((leftCount-rightCount) > 0)//will not go past the end of the string
					{
		       	  		   rightCount+=2;
					}
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
