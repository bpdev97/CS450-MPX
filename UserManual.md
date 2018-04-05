User Manual
===========
The purpose of this manual is to give users a summary of the commands that can be used in this operating system.

Commands
---------
Currently there are twenty commands including the help command that can be executed in this operating system.

1. help
2. shutdown
3. clear
4. setdate
5. getdate
6. settime
7. gettime
8. version
9. showPCB
10. showReady
11. showBlocked
12. showAll
13. createPCB
14. deletePCB
15. block
16. unblock
17. suspend
18. resume
19. setPriority
20. isEmpty
21. showFreeMemory
22. showAllocatedMemory


### help
*Usage:*  
Help is used by typing *help* followed by the command that you would like help with. A brief explanation of the usage and result of the command will be given. For example: *help setdate* will give information about the setdate command.

### shutdown
*Usage:*  
The shutdown command shuts down the operating system. Shutdown takes no arguments and will prompt the user to enter "Y" or "N" to choose whether to really shutdown or not.

### clear
*Usage:*  
The clear command effectively clears the terminal display. The clear command prints out many new line characters to move past outputs off the screen. Clear takes no arguments.

### setdate
*Usage:*  
The setdate command allows the user to input and set the system date. The setdate command follows American *Month/Day/Year* format. For example, today is the first of February so the input would look like: *setdate 2/1/18* or *setdate 02/01/18*.

### getdate
*Usage:*  
The getdate command will print out the current date. The getdate command can be used by simply typing *getdate*.

### settime
*Usage:*  
The settime command allows the user to input and set the system time. Settime only works with 24 hour time and the hour, minute, and second must all be entered for the input to be valid. For example: *settime 20:24:28* will set the time to the equivalent of 8:24:28 PM.

### gettime
*Usage:*  
The gettime command will retrieve and print out the current system time in 24 hour format. The gettime command can be used by typing *gettime*.

### version
*Usage:*  
The version command displays the current version of MPX and the completion date. The version command can be used by typing *version*.

### showPCB
*Usage:*  
The showPCB checks to see if the PCB name exists in the in the ready queue, readySuspended queue, blocked queue, or blockedSuspended queue.  If the PCB exists then the name, class, ready state, blocked state, and priority of the PCB is displayed. The showPCB command can be used by typing *showPCB* followed by the name of the PCB. For example *showPCB testName* will search the queues for a PCB with the name *testName*.

### showReady
*Usage:*  
The showReady command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue and readySuspended queue. The showReady command can be used by typing *showReady*.

### showBlocked
*Usage:*  
The showBlocked command displays the name, class, ready state, blocked state, and priority for all PCBs in the blocked queue and blockedSuspended queue. The showBlocked command can be used by typing *showBlocked*.

### showAll
*Usage:*  
The showAll command displays the name, class, ready state, blocked state, and priority for all PCBs in the ready queue, readySuspended queue, blocked queue, and blockedSuspended queue. The showAll command can be used by typing *showAll*.

### createPCB
*Usage:* 
The createPCB function creates a PCB with the user input name, class, and priority. The createPCB command can be used by typing createPCB.

### deletePCB
*Usage:* 
The deletePCB function deletes a PCB with the user input name. The deletePCB command can be used by typing deletePCB.

### block
*Usage:*
The block command can accept one argument, that being the name of a process. Block will find a process and change its state to blocked.

### unblock
*Usage:*
The unblock command can accept one argument, that being the name of a process. Unblock will find a process and change its state to unblocked.

### suspend
*Usage:*
The suspend command can accept one argument, that being the name of a process. Suspend will find a process and change its state to suspended.

### resume
*Usage:*
The resume command can accept one argument, that being the name of a process. Resume will find a process and change its state to resumed.

### setPriority
*Usage:*
The setPriority command can accept two arguments. The name of a process, and a priority to set in the process. The priority can be an integer between 0 and 9. SetPriority will find the process and then set the priority.

### yield
*Usage:*
The yield command takes in no arguments. It will make the command handler IDLE such that other processes can be run.

### loadr3
*Usage:*
The loadr3 command takes in no arguments. It creates five processes and adds them to the ready queue to be executed.

### isEmpty
*Usage:*
The isEmpty command takes in no arguments. It displays true if there is no allocated memory, false otherwise

### showFreeMemory
*Usage:*
The showFreeMemory command takes in no arguments. It displays the starting address and size of all the blocks of free memory

### showAllocatedMemory
*Usage:*
The showAllocatedMemory command takes in no arguments. It displays the starting address and size of all the blocks of allocated memory.
