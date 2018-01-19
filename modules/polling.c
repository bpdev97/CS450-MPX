#include <system.h>
#include <core/serial.h>
#include <polling.h>

/*
  Procedure..: poll
  Description..: 
  Params..: 
*/

int poll(char * buffer, int* count)
{
   int maxBufferSize = *count;
   count* = 0;
   char * exit = "exit";
   char * read = "read";
   char * write = "write";
   char * idle = "idle";
   char emptyChar = '';
   char newLine = '\n';
   int leftCount = 0; 
   int rightCount = 0;
   int inputCommand; 

/* Testing code to find int values of characters
	while(1)
	{
		if inb(COM1+5)&1 // Is a character available?
		{
           		char letter = inb(COM1); //Get the character
			serial_print(*letter);
		}
	}
*/

    while (1) // Run continuously
    {
	if inb(COM1+5)&1 // Is a character available?
	{
           char letter = inb(COM1); //Get the character
	   
	   if ((letter >=65 && letter <=90) //if upercase make lowercase
	      {
		letter = letter + 32;
	      }
	     
	    if ((letter >=97 && letter <=122) || (letter >=48 && letter <=57))//must be number/letter to add to buffer
	      {
		if((leftCount-rightCount) == 0) //arrow keys not used or cancel
		{	
		   buffer* = letter; //add letter to buffer
		   buffer++; //move the pointer of the buffer forward one
		   count*++; //increase counter
		}
		 
		else if((leftCount - rightCount) > 0)//Cursor is moved to the left
		{
 		  buffer -= (leftCount - rightCount); //moves pointer to where new character will be inserted
		  char temp1 = *buffer; //saves character being overwritten by new character
		  char temp2; //saves the next character in the buffer
		  buffer* = letter; // writes new letter overtop of the char saved in temp1 
		  for(int i = 0; i<(leftCount - rightCount); i++)
		   {
		     buffer++; // moves ptr over one
		     temp2 = *buffer; //saves next character in sequence
		     buffer* = temp1; // replaces character with char saved in temp 1
		     temp1 = temp2; // temp 1 equal temp 2

		   }
		  buffer++;// moves ptr to end of string

		}

		
	      }
	     
	    else //check for command keys
	     {
		//IMPORTANT, i'm not 100% sure about int equivalent of the character, needs to be checked
	       switch(letter) 
		 {
		   case 8 : //backspace, removes previous character
		     buffer--;//moves buffer back one
		     count*--;//decreases counter
		     buffer* = emptyChar;//I think this would erase the character
                   break;
		   
		  case 127 : // delete, deletes entire buffer
			for(int i =0; i<count*; i++)
			{
			    buffer* = emptyChar;//I think this would erase the character
			    buffer--;//moves buffer back one
			}
			 count* = 0;
		   break;

		 case 13 ://enter, checks for valid command
		   if (counter > 5)//all the valid commands are under length 6
		     {
		       inputCommand = 4;
		     }
		   else
		     {
		       if(checkCommand(buffer, exit, count*))
			 {
			   inputCommand = 0;
			 }
			else if(checkCommand(buffer, idle, count*))
			  {
			    inputCommand =1;
			  }
			else if(checkCommand(buffer, read, count*))
			  {
			    inputCommand = 2;
			  }
		        else if(checkCommand(buffer, write, count*))
			  {
			    inputCommand = 3;
			  }	  
		     }
		   break;
		   
		 case 10 :  //new line
		       buffer* = newLine; //add letter to buffer
		       rightCount = 0;//changes cursor to begining of new line
		       leftCount = 0;//changes cursor to begining of new line
		       buffer++; //move the pointer of the buffer forward one
		       count*++; //increase counter
		   break;

                //IMPORTANT These ints are 100% wrong but they are not on the ascii table
		 case 11 : //left arrow key
			if((leftCount-rightCount) < *count)//will not go before the buffer starts
			{
			   leftCount++;
 			}
		   break;

		case 12 : //right arrow key
			if((leftCount-rightCount) > 0)//will not go past the end of the string
			{
		       	   rightCount++;
			}
		   break;
		case 13: //carriage return
		leftCount = *count;
		rightCount = 0;
		break;
		 default :
		   //rest of keys, leave alone for now
		 }
	     }

	}
    }
	return inputCommand;
}

 int checkCommand(char * buffer, char * command, int counter) 
 {
   int bool = 1;
   
   buffer = buffer - counter;//starts at begining of string
   
      for(int i =0; i<counter; i++)//loops through but limit might cause errors
	{
	  if (*buffer != *(command+i)) //will probably throw a null pointer
	      {
		bool = 0;
	      }
	    
	   buffer++;  	    
	}
    buffer = buffer - counter;
      return bool;			     
 }
