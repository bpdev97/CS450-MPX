#include <system.h>
#include <core/serial.h>
#include <core/io.h>
#include "polling.h"
#include "mpx_supt.h"

/*
  Procedure..: poll
  Description..: 
  Params..: 
*/

int poll(char * buffer, int* count)
{
   char newBuffer[100]; //size will be alocated later
   char letter[2];
   int counter = 0;
   int maxBufferSize = *count;
   int exit =0;
   int enter = 1;
   char emptyChar = ' ';
   char backspace[] = {'\b',' ','\b','\0'};
/*
   count = &counter;
   char emptyChar = ' ';
   char newLine = '\n';
   int leftCount = 0; 
   int rightCount = 0; */

/*  WHAT NEEDS TO BE DONE
   -Character Values for delete, newline, and arrow keys needed for case statement
   -delete, newline, and arrow keys implemetation, i have a good idea for how that is done but have't done it yet
   -how to manipulate console line when backspace, delete, and arrow keys are pressed, no idea on how that works
*/

   while(enter)
    {
      
        if (inb(COM1+5)&1) // Is a character available?
        {
            letter[0] = inb(COM1);// stores single character to print to com1
            letter[1] = '\0';
     	      
	    if ( (letter[0] >=97 && letter[0] <=122) || (letter[0] >=65 && letter[0] <=90) || (letter[0] >=48 && letter[0] <=57) )//checks for charater or number
		{
	  	  newBuffer[counter] = letter[0]; //adds single character to buffer
	 	  counter++;
	  	  newBuffer[counter] = letter[1];// adds null terminate
	   	  counter++;
		  int count = 2; //used only to print the single character
                  sys_req(WRITE, COM1, letter, &count);//prints single character from input
		}
	    else
		{
		     switch(letter[0])
		     {
			 int i;

		         case '\r': //enter, works, reprints the buffer in whole and terminates polling
	                 for(i=0; i<counter; i++)
	   	         {
 	    	           sys_req(WRITE, COM1, &newBuffer[i], &count);
          	         }    
			 enter =0;                                 
		     	break;
 			
			case 127 : //backspace, works in memory but not on console line
		     	counter--;
			newBuffer[counter] = emptyChar;
			counter--;
			newBuffer[counter] = emptyChar;
			int backCount = 4;
 	    	        sys_req(WRITE, COM1, &backspace, &backCount);
			break;

			case 8: // delete,does not work, prints '3'
			 klogv("delete");
			
		        break;
			
			case '\n' :  //new line, also does not work
			klogv("newline");
		      
		  	 break;

			case 37 : //left arrow key, prints 'D'
			klogv("left arrow key");
			
		   	break;

			case 39 : //right arrow key, prints 'C'
			klogv("right arrow key");
			
		        break;

	
                     }
		}		
        }
	
    }	
	
	count = &counter;   
	buffer = newBuffer;
	return 0;
}

 
