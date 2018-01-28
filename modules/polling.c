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
/*
TO BE DONE
  Need to fix console line for delete and backspace
  Probably need some more error handling
  Delete and insert is actually bigger than a byte which is a problem we need to discuss
  New line implementation?
*/

int poll(char * buffer, int* count) {
	char letter[4];
	int counter = 0;
	int maxBufferSize = *count;
	int enter = 0;
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
			if ( (letter[0] != BACKSPACE) && (letter[0] != ENTER) && (letter[0] != '\033') &&  (strcmp(letter, "~\000\000") != 0) && (counter < maxBufferSize))
			 {
				if((leftCount-rightCount) == 0)
				{
				   
				   buffer[counter] = letter[0]; // Add character to buffer and then increment counter
 			           counter++;//increase counter
				   int count = 4;
				   sys_req(WRITE, COM1, letter, &count); // Write the character to COM1
				}

				else if((leftCount - rightCount) > 0)//Cursor is moved to the left
				{
				   
 		 		   counter -= ((leftCount - rightCount)); //moves pointer to null terminator left of char to be inserted
		 		   char temp1 =  buffer[counter]; //saves character being overwritten by new character
		 		   char temp2; 			    //saves the next character in the buffer
		 		   buffer[counter] = letter[0]; // writes new letter overtop of the char saved in temp1 
		 		   int i;
				   for(i = 0; i<(leftCount - rightCount); i++)
		                   {
		    			 counter++; // moves ptr over to next character
		    			 temp2 = buffer[counter]; //saves next character in sequence
		    			 buffer[counter] = temp1; // replaces character with char saved in temp 1
		     			 temp1 = temp2; // temp 1 equal temp 2
		  		   }
				  counter++;
			          buffer[counter] = temp1;//adds last character
				  
			          int count = 4;
				  sys_req(WRITE, COM1, letter, &count); // Write the character to COM1
				}
				
			}

			// Handle special case characters
			else if(counter != 0){
				// Enter Recieved
				if(strcmp(letter, "\r\000\000") == 0) {
					
					   int i = 0;
					   serial_println("");
					   buffer[counter] = '\0';
					   enter = 1;     
					                          
				}
					
				// Backspace Recieved
				else if(strcmp(letter, "\177\000\000") == 0) {	
					if((leftCount - rightCount) == 0)
					{ 
				          counter--;
					  buffer[counter] = '\0'; 
					}
					else if((leftCount - rightCount) > 0)
					{
			                 counter -= ((leftCount - rightCount)+1); //moves pointer to  char that is to be deleted
		 		   	 int i;
				  	 for(i = 0; i<(leftCount - rightCount); i++)
		                  	 {
					   buffer[counter] = buffer[counter+1];//shifts characters to the left
		    		           counter++;
		  		   	 }
					}
					int backCount = 4;
					sys_req(WRITE, COM1, &backspace, &backCount);
				}
				
				// Delete recieved
				else if (strcmp(letter, "\033[3") == 0) {
					 
					//delete is bigger than 4 long "\033[3" and "~\000\000"
					//insert is "\033[3" and ";2~"


					/*int count = 4; //Does not work
					sys_req(WRITE, COM1, letter, &count);*/
					if((leftCount - rightCount) > 0)
					{
			                 counter -= ((leftCount - rightCount)); //moves pointer to  char that is to be deleted
		 		   	 int i;
				  	 for(i = 0; i<((leftCount - rightCount)); i++)
		                  	 {
						buffer[counter] = buffer[counter+1]; //moves characters to the left
		    			 	counter++;
		  		   	 }
					  counter--;	
					  leftCount--;//moves cursor to the right one
					}
				}

				// Left arrow Key recieved
				else if (strcmp(letter, "\033[D") == 0) {
					
					if((leftCount-rightCount) < counter)//will not go before the buffer starts
					{
			   		   leftCount ++;
					   int count = 4;
					   sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
 					}
				}
				// Right arrow Key recieved
				else if (strcmp(letter, "\033[C") == 0) {
					if((leftCount-rightCount) > 0)//will not go past the end of the string
					{
		       	  		   rightCount++;
					   int count = 4;
					   sys_req(WRITE, COM1, letter, &count); // Write letter to COM1
					}
					
				}
			}
		}
	}
	
	// Enter pressed
	count = &counter;  
	return 0;
}

