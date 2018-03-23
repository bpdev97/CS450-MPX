#include <system.h>
#include <core/serial.h>
#include <core/io.h>
#include <string.h>
#include "polling.h"
#include "mpx_supt.h"

/*
  Procedure..: poll
  Description..: Takes keyboard input and checks to see if it is a character or command key.  Characters are stored in the buffer and commands modify the buffer
  Params..: pointer to buffer and pointer to buffersize 
*/
char backspace[] = {'\b',' ','\b','\0'};


int poll(char * buffer, int* count) {
	char letter[5];
	int counter = 0;
	int maxBufferSize = *count;
	int enter = 0;
	int cursorPosition = 0;

	while(!enter) {  
    	if (inb(COM1+5)&1) { // Check for character
            letter[0] = inb(COM1); // Build letter from inb(COM1)
            letter[1] = inb(COM1);
            letter[2] = inb(COM1);
	    letter[3] = inb(COM1);
            letter[4] = '\0';
			// Look for special case characters
			if ( (letter[0] != BACKSPACE) && (letter[0] != TAB) &&(letter[0] != ENTER) && (letter[0] != '\033') && (counter < maxBufferSize)) {
				if(cursorPosition == 0){ //if cursor is at the end of the buffer
				   buffer[counter] = letter[0]; // Add character to buffer and then increment counter
 			           counter++;//increase counter
				   print(letter); // Write the character to COM1
				}

				else if(cursorPosition> 0){//Cursor is moved to the left
				   clearCommandLine(counter,cursorPosition);  
 		 		   counter -= (cursorPosition); //moves pointer to left of char to be inserted
		 		   char temp1 =  buffer[counter]; //saves character being overwritten by new character
		 		   char temp2; 			    //saves the next character in the buffer
		 		   buffer[counter] = letter[0]; // writes new letter overtop of the char saved in temp1 
		 		   int i;
				   
				   //shifts the rest of the characters to the right
				   for(i = 0; i<cursorPosition; i++) {
		    			 counter++; // moves ptr over to next character
		    			 temp2 = buffer[counter]; //saves next character in sequence
		    			 buffer[counter] = temp1; // replaces character with char saved in temp 1
		     			 temp1 = temp2; // temp 1 equal temp 2
		  		   }

				  counter++;
			          buffer[counter] = temp1;//adds last character
			          print(buffer); //reprints new buffer
				  resetCursor(cursorPosition);
				}
				
			}

			// Handle special case characters
			else if(counter > 0){
				
				// Enter Recieved
				if(strcmp(letter, "\r\000\000\000") == 0) {
					   print("\r");
					   buffer[counter] = '\0';
					   enter = 1;                           
				}
					
				// Backspace Recieved
				else if(strcmp(letter, "\177\000\000\000") == 0 && ((counter - cursorPosition)>0) ) {	

					if(cursorPosition == 0){ //cursor is at end of buffer
				          counter--;
					  print(backspace);
					  buffer[counter] = '\0'; //writes over last char with null terminator
					}

					else if(cursorPosition> 0){//cursor shifted to left
					 clearCommandLine(counter,cursorPosition);  
			                 counter -= (cursorPosition+1); //moves pointer to char that is to be deleted
		 		   	 int i;
				          
					 //shifts characters past the one being deleted to the left
				  	 for(i = 0; i<cursorPosition; i++){
					   buffer[counter] = buffer[counter+1];//shifts characters to the left
		    		           counter++;
		  		   	 }

					 buffer[counter] = '\0'; //replaces last character with null terminator so it is not repeated
					 print(buffer); //reprints new buffer
					 resetCursor(cursorPosition);
					}
				}
				
				// Delete recieved
				else if (strcmp(letter, "\033[3~") == 0) {
					 
					if(cursorPosition > 0 && counter >1){ //must have more than one char and cursor is shifter
					 clearCommandLine(counter,cursorPosition);
			                 counter -= cursorPosition; //moves pointer to  char that is to be deleted
		 		   	 int i;
					 
					 //shifts characters past the one being deleted to the left
				  	 for(i = 0; i<cursorPosition; i++){
						buffer[counter] = buffer[counter+1]; //moves characters to the left
		    			 	counter++;
		  		   	 }
					  counter--;
					  cursorPosition--;//moves cursor to the right one
					  print(buffer); //reprints new buffer
					  resetCursor(cursorPosition);
					}
					else if(cursorPosition > 0 && counter == 1){ //if there is only one character left
					   buffer[0] = '\0';
					   clearCommandLine(counter,cursorPosition);
					   counter--;
					   cursorPosition = 0;
					}
				        
				}

				// Left arrow Key recieved
				else if (strcmp(letter, "\033[D\000") == 0) {
					
					if(cursorPosition < counter){//will not go before the buffer starts
			   		   cursorPosition++;
				           print(letter);
 					}
				}
				// Right arrow Key recieved
				else if (strcmp(letter, "\033[C\000") == 0) {

					if(cursorPosition > 0){//will not go past the end of the buffer
		       	  		   cursorPosition--;
					   print(letter);
					}
					
				}
			}
		}
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}
	
	// Enter pressed
 	count = &counter;
	return 0;
}

/*
  Procedure..: clearCommandLine
  Description..:clears the buffer from the terminal 
  Params..: number of characters and the position of the cursor 
*/
void clearCommandLine(int counter,  int cursorPosition){
  int i;
  for(i=0; i<cursorPosition;i++){
	print("\033[C\000");
  }
  for(i=0; i<counter; i++){
	print(backspace);
  }
}
/*
  Procedure..: resetCursor
  Description..:moves the cursor to appropriate position after a new buffer is reprinted to the terminal
  Params..: original position of the cursor 
*/
void resetCursor(int cursorPosition){
  int i;
  for(i=0; i<cursorPosition;i++) {
	  print("\033[D\000");
  }
}

