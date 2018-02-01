#ifndef _STRING_H
#define _STRING_H

#include <system.h>

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c);

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n);

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2);

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2);

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int   strlen(const char *s);

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int   strcmp(const char *s1, const char *s2);

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2);

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s);

/*
  Procedure..: swap
  Description..: Swaps two charcters
  Params..: char *x-single char, char*y-single char
*/
void swap(char *x, char *y);

/*
  Procedure..: reverse
  Description..: Reverses a string
  Params..: char *buffer-string buffer, int i-first position, int j-last position
*/
char *reverse(char *buffer, int i, int j);

/*
  Procedure..: abs
  Description..: Returns absolute value of signed int
  Params..: int x-signed int
*/
int abs(int x);

/*
  Procedure..: itoa
  Description..: Converts an integer to an ASCII string
  Params..: int value-integer to convert, char *str-output string, int base-number base
*/
char *itoa(int value, char *str, int base);

/*
  Procedure..: sprintf
  Description..: Formats a string using tags and given a list of arguments
  Params..: char *str-output string, const char *format-format string, ...-arbitrary list of arguments
*/
int sprintf(char *str, const char *format, ...);



// int sprintfv(char *str, const char *format, va_list ap);
//int printf(const char *format, ...);

#endif
