#include <system.h>
#include <string.h>
#include <stdarg.h>
#include "../modules/mpx_supt.h"

/* ************************************ *
 * Author:  Forrest Desjardins    	*
 * Creation Date:  2014           	*
 * 				  	*
 *   DISCLAIMER!!!!!:  		  	*
 *  The following methods are provided	*
 *  for your use as part of the MPX	*
 *  project. While we have tested these	*
 *  We make certification of 	 	*
 *  complete correctness.		*
 *   Additionally not all methods are 	*
 *  implemented :  See:  itoa		*
 * ************************************ */


/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char *s)
{
  int r1 = 0;
  if (*s) while(*s++) r1++;
  return r1;//return length of string
}

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2)
{
  char *rc = s1;
  while( (*s1++ = *s2++) );
  return rc; // return pointer to destination string
}

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s)
{
  int res=0; 
   int charVal=0;
   char sign = ' '; 
   char c = *s;
   

   while(isspace(&c)){ ++s; c = *s;} // advance past whitespace

   
   if (*s == '-' || *s == '+') sign = *(s++); // save the sign
   
   
   while(*s != '\0'){ 
        charVal = *s - 48;
 	res = res * 10 + charVal; 
	s++;

   } 

   
   if ( sign == '-') res=res * -1;

  return res; // return integer
}
/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char *s1, const char *s2)
{

  // Remarks:
  // 1) If we made it to the end of both strings (i. e. our pointer points to a
  //    '\0' character), the function will return 0
  // 2) If we didn't make it to the end of both strings, the function will
  //    return the difference of the characters at the first index of
  //    indifference.
  while ( (*s1) && (*s1==*s2) ){
    ++s1;
    ++s2;
  }
  return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
}


/* ---------------------------------------
    Functions below this point are given.
    No need to tamper with these!
   --------------------------------------- */

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2)
{
  char *rc = s1;
  if (*s1) while(*++s1);
  while( (*s1++ = *s2++) );
  return rc;
}

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c)
{
  if (*c == ' '  ||
      *c == '\n' ||
      *c == '\r' ||
      *c == '\f' ||
      *c == '\t' ||
      *c == '\v'){
    return 1;
  }
  return 0;
}

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n)
{
  unsigned char *p = (unsigned char *) s;
  while(n--){
    *p++ = (unsigned char) c;
  }
  return s;
}

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2)
{
  static char *tok_tmp = NULL;
  const char *p = s2;

  //new string
  if (s1!=NULL){
    tok_tmp = s1;
  }
  //old string cont'd
  else {
    if (tok_tmp==NULL){
      return NULL;
    }
    s1 = tok_tmp;
  }

  //skip leading s2 characters
  while ( *p && *s1 ){
    if (*s1==*p){
      ++s1;
      p = s2;
      continue;
    }
    ++p;
  }

  //no more to parse
  if (!*s1){
    return (tok_tmp = NULL);
  }

  //skip non-s2 characters
  tok_tmp = s1;
  while (*tok_tmp){
    p = s2;
    while (*p){
      if (*tok_tmp==*p++){
	*tok_tmp++ = '\0';
	return s1;
      }
    }
    ++tok_tmp;
  }

  //end of string
  tok_tmp = NULL;
  return s1;
}

//Basic function to swap two characters.
void swap(char *x, char *y){
    char temp = *x;
    *x = *y;
    *y = temp;
}

//Function to reverse the character buffer that will be given to itoa
char *reverse(char *buffer, int i, int j){
    //This loop iteratively steps and swaps the first and last character in the buffer
    while(i<j){
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}

int abs(int x){
    x = (x < 0)? x*-1 : x;
    return x;
}

/**
 * itoa converts an integer value to a string using the given base and stores the result.
 * If the base is 10 and the value is negative then the string will have a leading minus
 * sign. With all other bases the value is considered unsigned. The number base is 
 * limited between 2 and 36. The returned string is null terminated.
*/
char *itoa(int value, char *str, int base){
    //Reject bases outside of the range
    if(base < 2 || base > 36){
        return str;
    }

    //This keeps track of the index in the return string
    int i = 0;

    //Handle the zero case separately and take the absolute value of the number
    int n = abs(value);
    if(n == 0){
        str[i++] = 0;
        str[i] = '\0';
        return str;
    }

    /**
     * Since itoa is defined to allow users to convert to arbitrary number bases we
     * have to do that before converting to a string. The way to do this is to continually
     * take the modulo of the original number and append the remainder to the string while
     * dividing the original number by the base. You do this until the original number is not
     * greater than 0.
    */
    while(n > 0){
        int remainder = n % base;
        /**
         * If the remainder is greater than 9 then it cannot be represented by a single number.
         * Therefore, characters A-Z are used. In ASCII, '0' is 48 and 'A' is 65. These numbers
         * are used as offsets to get the correct ASCII characters. This behavior is easily
         * represented with a ternary operator.
        */
        str[i++] = (remainder > 9)? 65 + (remainder - 10) : 48 + remainder;
        n = n/base;
    }

    //If the number base is 10 and the number is negative a '-' is appended
    if(base == 10 && value < 0){
        str[i++] = '-';
    }
    str[i] = '\0';

    //When using the remainder method to convert number bases, the result has to be reversed to
    //get the correct answer
    
    return reverse(str, 0, i-1);
}

/**************************************************
 * sprintf takes in a blank character array, a string with or without format tags,
 * and as many arguments as there are tags. sprintf formats the given
 * string and outputs it into the blank character array. The return value is an integer
 * value representing the number of character written to the character array.
 * Format tags that work as of now include, integers, hexadecimal, octal,
 * single characters, and strings.
**************************************************/
int sprintf(char *str, const char *format, ...){
    //va_lists are how you pass in arbitrary amounts of arguments
    va_list ap;
    va_start(ap, format);

    char *index = str; //Index represents the end of the output string
    int i = 0; //Used to store the current argument from va_list
    int length = 0; //This is used to track how many characters are being concatenated onto the output string
    char s[33]; //Temp string storage for itoa output
    char *s2; //Used to store the current argument from va_list

    /*********************************************
     * These loops don't make a ton of sense right now but
     * they work. I will come back and clean them up later if
     * there is time. -Addison
    *********************************************/
    while(*format != '\0'){
        while(*format != '%' && *format != '\0'){
            *index++ = *format++;
        }
        if(*format == '\0'){
          // *index = '\0';
          break;
        }
        format++;
        
        switch(*format){

            //Decimal/Integer argument
            case 'd':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Alternate decimal/integer argument
            case 'i':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Hex argument
            case 'x':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 16));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Octal argument
            case 'o':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 8));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Character argument
            case 'c':
                i = va_arg(ap, int);
                *index++ = i;
                break;

            //String argument
            case 's':
                s2 = va_arg(ap, char*);
                length = strlen(s2);
                strcat(index, s2);
                index += length;
                break;

            //Default case just breaks out if the tag is bad
            default:
              break;
        }

        format++;
    }
    va_end(ap);
    return strlen(str);
}


//EVERYTHING AFTER THIS POINT IS EXPERIMENTAL AND INCOMPLETE


/*
int sprintfv(char *str, const char *format, va_list ap){
    // va_list ap;
    // va_start(ap, format);

    char *index = str; //Index represents the end of the output string
    int i = 0;
    int length = 0;
    char s[33];
    char *s2;

    while(*format != '\0'){
        while(*format != '%' && *format != '\0'){
            *index++ = *format++;
        }
        if(*format == '\0'){
          // *index = '\0';
          break;
        }
        format++;
        
        switch(*format){

            //Decimal/Integer argument
            case 'd':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Alternate decimal/integer argument
            case 'i':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Hex argument
            case 'x':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 16));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Octal argument
            case 'o':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 8));
                length = strlen(s);
                strcat(index, s);
                index += length;
                memset(s, 0, 33);
                break;

            //Character argument
            case 'c':
                i = va_arg(ap, int);
                *index++ = i;
                break;

            //String argument
            case 's':
                s2 = va_arg(ap, char*);
                length = strlen(s2);
                strcat(index, s2);
                index += length;
                break;

            default:
              break;
        }

        format++;
    }
    va_end(ap);
    return strlen(str);
}
*/

/*
int printf(const char *format, ...){
  char s[128];
  int len = 0;
  va_list ap;

  va_start(ap, format);
  len = sprintfv(s, format, ap);
  print(s);
  return len;
}
*/


