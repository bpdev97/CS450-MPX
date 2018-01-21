#include <system.h>
#include <core/serial.h>
#include <core/io.h>

/*
  Procedure..: poll
  Description..: 
  Params..: 
*/

int poll(char * buffer, int* count)
{
   int maxBufferSize = *count;
   int counter = 0;
   int enter = 1;
   count = &counter;
   char emptyChar = ' ';
   char newLine = '\n';
   int leftCount = 0; 
   int rightCount = 0;
   int i;
 

    while (enter) // Run continuously
    {
	if (inb(COM1+5)&1) // Is a character available?
	{
           unsigned char letter = (inb(COM1)); //Get the character
	   int letterNum = (int) letter;
	     
	    if ( (letterNum >=97 && letterNum <=122) || (letterNum >=65 && letterNum <=90) || (letterNum >=48 && letterNum <=57) )//must be number/letter to add to buffer
	    {
		if((leftCount-rightCount) == 0) //arrow keys not used or cancel
		{	
		   buffer = &letter; //add letter to buffer
		   klogv(buffer);
		   buffer++; //move the pointer of the buffer forward one		   
		   counter++;
		   count = &counter; //increase counter
		   
		}
		 
		else if((leftCount - rightCount) > 0)//Cursor is moved to the left
		{
 		  buffer -= (leftCount - rightCount); //moves pointer to where new character will be inserted
		  char temp1 = *buffer; //saves character being overwritten by new character
		  char temp2; //saves the next character in the buffer
		  buffer = &letter; // writes new letter overtop of the char saved in temp1 
		  for(i = 0; i<(leftCount - rightCount); i++)
		   {
		     buffer++; // moves ptr over one
		     temp2 = *buffer; //saves next character in sequence
		     buffer = &temp1; // replaces character with char saved in temp 1
		     temp1 = temp2; // temp 1 equal temp 2

		   }
		  buffer++;// moves ptr to end of string
		  counter++;
		  count = &counter; //increase counter

		}

		
	      }
	     
	    else //check for command keys
	     {
	       switch(letterNum) 
		 {
		   case 127 : //backspace, removes previous character
		     klogv("backspace");
		     buffer--;//moves buffer back one
		     counter--;
		     count = &counter; //decrease counter
		     buffer = &emptyChar;//I think this would erase the character
                   break;
		   
		  case 8: // delete, deletes entire buffer
			 klogv("delete");
			for(i = 0; i<counter; i++)
			{
			    buffer = &emptyChar;//I think this would erase the character
			    buffer--;//moves buffer back one
			}
			counter = 0;
  			count = &counter; //reset counter
		   break;

		 case 13 ://enter, checks for valid command
		      klogv("enter");
		     buffer = buffer - counter;
		     klogv(buffer);
		     enter = 0;
		   break;
		   
		 case 10 :  //new line, 
			klogv("newline");
		       buffer = &newLine; //add letter to buffer
		       rightCount = 0;//changes cursor to begining of new line
		       leftCount = 0;//changes cursor to begining of new line
		       buffer++; //move the pointer of the buffer forward one
		       counter++;
  		       count = &counter; //increase counter
		       
		   break;

                //IMPORTANT These ints are 100% wrong but they are not on the ascii table
		 case 37 : //left arrow key
			klogv("left arrow key");
			if((leftCount-rightCount) < *count)//will not go before the buffer starts
			{
			   leftCount++;
 			}
		   break;

		case 39 : //right arrow key
			klogv("right arrow key");
			if((leftCount-rightCount) > 0)//will not go past the end of the string
			{
		       	   rightCount++;
			}
		   break;

		 }
	     }

	}
    }

	return *count;
}

 /*
int counter = 0;
	while(counter <10)
	{
		if (inb(COM1+5)&1)// Is a character available?
		{
           		char letter = (inb(COM1)); //Get the character
			int letterNum = (int) letter;
			klogv(&letterNum);
			//serial_printNum(&letterNum);
			counter++;
		}
		
	}
*/
