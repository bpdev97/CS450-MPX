Programmer's Manual
===================
The purpose of this manual is to give information on all of the functions included in this MPX. Each .c file will have its own section for the functions contained in it.

Functions - processcommand.c
--------------------------
proccesscommand.c contains the functions for all of the commands that can be executed in MPX.

### help()
*Description:*  
The help function is used to print help messages for commands.

*Declaration:*  
void help(int numword, char* args[]);

*Parameters:*  
numword - represents the number of arguments passed from the command line.  
args - an array of character pointers containing the arguments from the command line. Help can only take one argument and it will check to see if that argument corresponds to a command with a help message. If no argument is given, help will print out a list of possible commands that it can help with.

*Return Value:*  
help has no return value.

### version()
*Description:*  
The version function prints the current version of MPX.

*Declaration:*  
void version(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. Version consumes no arguments.

*Return Value:*  
version has no return value.

### settime()
*Description:*  
The settime function sets the system time in 24 hour format. If an improper time string is passed to settime, an error message will be given.

*Declaration:*  
void settime(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. Settime can only take one argument from the command line. That argument is a string in the format *hour:minute:second:*.

*Return Value:*  
settime has no return value.

*Example:*  
settime 15:30:45 sets the time to the equivalent of 3:30:45 PM.

### gettime()
*Description:*  
The gettime function prints the current system time.

*Declaration:*  
void gettime(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. Gettime consumes no arguments.

*Return Value:*  
gettime has no return value.

### setdate()
*Description:*  
The setdate function sets the system date.

*Declaration:*  
void setdate(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. Setdate can only take one argument from the command line. That is a string in the format *Month/Day/Year*. *Year* must be in two digit format.

*Return Value:*  
setdate has no return value.

### getdate()
*Description:*  
The getdate function prints the current system date in the format *Month/Day/Year*.

*Declaration:*  
void getdate(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. The getdate function consumes no arguments.

*Return Value:*  
getdate has no return value.

### tobcd()
*Description:*  
The tobcd function take a binary value and converts it to binary coded decimal.

*Declaration:*  
int tobcd(int binary);

*Parameters:*  
binary - binary value to be converted

*Return Value:*  
Returns a BCD value

### printbcd()
*Description:*  
Takes in a BCD value and prints out its ASCII representation

*Declaration:*  
void printbcd(int bcd);

*Parameters:*  
bcd - BCD value to be printed

*Return Value:*  
No return value.

### showPCB()
*Description:*  
The showPCB function searches the ready and blocked queues for a PCB name and if found, prints the name, class, ready state, blocked state, and priority of the PCB.  If the PCB name is not found in the queues then a error message is displayed.

*Declaration:*  
void showPCB(int argc, char* argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line. The PCB name is found in argv[1]

*Return Value:*  
showPCB has no return value.

### showReady()
*Description:*  
The showReady function prints the name, class, ready state, blocked state, and priority of every PCB in the ready queue and readySuspended queue.

*Declaration:*  
void showReady();

*Parameters:*  
No parameters

*Return Value:*  
showReady has no return value.

### showBlocked()
*Description:*  
The showBlocked function prints the name, class, ready state, blocked state, and priority of every PCB in the blocked queue and blockedSuspended queue.

*Declaration:*  
void showBlocked();

*Parameters:*  
No parameters

*Return Value:*  
showBlocked has no return value.

### showAll()
*Description:*  
The showBlocked function prints the name, class, ready state, blocked state, and priority of every PCB in the ready, suspendedReady, blocked, and blockedSuspended queues.

*Declaration:*  
void showAll();

*Parameters:*  
No parameters

*Return Value:*  
showAll has no return value.

### CreatePCB()
*Description:*  
The CreatePCB function creates a PCB with the user input name, class, and priority.

*Declaration:*  
void CreatePCB(int argc, char *argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*  
CreatePCB has no return value.

### DeletePCB()
*Description:*  
The DeletePCB function deletes a PCB with the user input name.

*Declaration:*  
void DeletePCB(int argc, char *argv[]);

*Parameters:*  
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*  
DeletePCB has no return value.

### blockPCB()
*Description:*
The blockPCB function finds a PCB, changes its ready state to blocked (-1) and then inserts it into the proper queue.

*Declaration:*
int blockPCB(int argc, char *argv[]);

*Parameters:*
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*
Returns an integer representing the success or failure of the function. Returns 1 for success, 0 for failure.

### unblockPCB()
*Description:*
The unblockPCB function finds a PCB, changes its ready state to ready (1) and then inserts it into the proper queue.

*Declaration:*
int unblockPCB(int argc, char *argv[]);

*Parameters:*
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*
Returns an integer representing the success or failure of the function. Returns 1 for success, 0 for failure.

### suspendPCB()
*Description:*
The suspendPCB function finds a PCB, changes its suspend state to suspended (1) and then inserts it into the proper queue.

*Declaration:*
int suspendPCB(int argc, char *argv[]);

*Parameters:*
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*
Returns an integer representing the success or failure of the function. Returns 1 for success, 0 for failure.

### resumePCB()
*Description:*
The resumePCB function finds a PCB, changes its suspend state to resumed (0) and then inserts it into the proper queue.

*Declaration:*
int resumePCB(int argc, char *argv[]);

*Parameters:*
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*
Returns an integer representing the success or failure of the function. Returns 1 for success, 0 for failure.

### setPriority()
The setPriority function finds a PCB, changes its priority to the priority given in argv and then inserts it into the proper queue.

*Declaration:*
int setPriority(int argc, char *argv[]);

*Parameters:*
argc - represents the number of arguments passed from the command line.  
argv - an array of character pointers containing the arguments from the command line.

*Return Value:*
Returns an integer representing the success or failure of the function. Returns 1 for success, 0 for failure.

Functions - polling.c
---------------------
polling.c contains the functions necessary to poll user input from the com port.

### poll()
*Description:*  
The poll function takes keyboard input and checks to see if it is a character or a command key.

*Declaration:*  
int poll(char *buffer, int *count);

*Parameters:*  
buffer - pointer to a character buffer  
count - pointer to the size of the buffer  

*Return Value:*  
poll returns 0.

### clearCommandLine()
*Description:*  
Clears the character buffer from the command line.

*Declaration:*  
void clearCommandLine(int counter, int cursorPosition);

*Parameters:*  
counter - number of characters on the line
cursorPosition - position of the cursor on the line

*Return Value:*  
No return value.

### resetCursor()
*Description:*  
Moves the cursor to the original position after a new buffer is printed to the terminal

*Declaration:*  
void resetCursor(int cursorPosition);

*Parameters:*  
cursorPosition - original position of the cursor

*Return Value:*  
No return value.

Functions - comhand.c
----------------------
comhand.c contains the functions necessary to handle commands in MPX

### comhand()
*Description:*  
comhand is a control functions that initializes several other systems. It sets up the polling function, handles the command prompt, and handles commands and arguments. It also facilitates the shutdown of the operating system.

*Declaration:*  
int comhand();

*Parameters:*  
No parameters.

*Return Value:*  
Returns 0 when exiting the operating system.

Functions - string.c
--------------------
string.c contains a multitude of functions used to manipulate strings.

### swap()
*Description:*  
swap swaps two characters.

*Declaration:*  
void swap(char *x, char *y);

*Parameters:*  
x - pointer to first character
y - pointer to second character

*Return Value:*  
No return value.

### reverse()
*Description:*  
reverse is given a character pointer and the first and last positons of the string. Reverse then reverses all of the characters in the string.

*Declaration:*  
char *reverse(char *buffer, int i, intj);

*Parameters:*  
buffer - pointer to the string to be reversed
i - first position
j - second position

*Return Value:*  
reverse returns the reversed character buffer.

### abs()
*Description:*  
abs returns the absolute value of a signed int.

*Declaration:*  
int abs(int x);

*Parameters:*  
x - signed integer value

*Return Value:*  
Returns absolute value of input.

### itoa()
*Description:*  
itoa converts an integer value to a string using a given number base and then stores and returns the result.

*Declaration:*  
char *itoa(int value, char *str, int base);

*Parameters:*  
value - integer value to be converted
str - pointer to buffer for string storage
base - number base to convert to

*Return Value:*  
itoa returns the number string

### sprintf()
*Description:*  
sprintf takes in a blank character array, a string with format tags, and then an arbitrary number of arguments. sprintf formats a string given these inputs and saves and returns the result. Arguments are formatted into the string using format tags. Integers are formated using %d or #i, hex uses %x, octal uses %o, single characters use %c, and strings use %s.

*Declaration:*  
int sprintf(char *str, const char *format, ...);

*Parameters:*  
str - character array that ouput is stored in
format - string containing format tags
... - list of arguments to be formatted into the string

*Return Value:*  
Returns a formatted string.

Functions - mpx_supt.c
----------------------
mpx_supt.c contains supplemental functions for MPX

### print()
*Description:*  
Prints a character string to the terminal with no new line character.

*Declaration:*  
void print(char *string);

*Parameters:*  
string - character pointer to string that will be printed

*Return Value:*  
No return value.

### println()
*Description:*  
Prints a character string to the terminal with a new line character.

*Declaration:*  
void println(char *string);

*Parameters:*  
string - character pointer to string that will be printed

*Return Value:*  
No return value.

Functions - pcb.c
----------------------
pcb.c contains functions for the pcb

### AllocatePCB()
*Description:*  
Allocates memory for a new PCB.

*Declaration:*  
PCB* AllocatePCB();

*Parameters:*  
No parameters

*Return Value:*  
Returns a PCB Pointer when successful and NULL when not. 

### FreePCB()
*Description:*  
Frees all memory associated with a given PCB.

*Declaration:*  
int FreePCB(PCB* freepcb);

*Parameters:*  
PCB Pointer to the pcb to be freed

*Return Value:*  
Returns an integer when it is successful and when it is not. 

### SetupPCB()
*Description:*  
Sets the PCB state to ready.

*Declaration:*  
PCB* SetupPCB(char* name, int classNum, int priority);

*Parameters:*  
Process Name, Process Class, and Process Priority

*Return Value:*  
Returns a PCB Pointer when successful and NULL when not. 

### FindPCB()
*Description:*  
Searches through all queues, finds the requested PCB, and returns a pointer to that pcb.

*Declaration:*  
PCB* FindPCB(char* name);

*Parameters:*  
Process Name

*Return Value:*  
Returns a PCB Pointer when successful and NULL when not. 

### InsertPCB()
*Description:*  
Inserts a PCB into the correct queue.

*Declaration:*  
InsertPCB(PCB *pcb);

*Parameters:*  
PCB to be inserted pointer

*Return Value:*  
Void

### RemovePCB()
*Description:*  
Finds and removes a PCB from the queue it is currently in.

*Declaration:*  
int RemovePCB(PCB *pcb);

*Parameters:*  
PCB to be removed pointer

*Return Value:*  
Returns 0 when successful and -1 when it fails to remove the given PCB.
