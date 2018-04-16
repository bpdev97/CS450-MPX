R6 Programmer's Manual
===================
The purpose of this manual is to give information on all of the functions included in R6. 

Functions 
--------------------------

### loadInitialDirectory()
*Description:*  
The loadInitalDirectory function is used to load the root directory into the current directory of the program at the start of the program.

*Declaration:*  
void loadInitialDirectory(char* disk)

*Parameters:*  
disk- the name of the FAT12 image being read.  

*Return Value:*  
loadInitialDirectory() has no return value.


### loadFAT()
*Description:*  
The loadFAT function is used to load the FAT table into an integer array. It reads the boot sector for the sectorsPerFat and bytesPerSector to initalize the FAT array in memory.  Then it reads the FAT table in the disk image and converts the bytes using little endian style and loads the converted bytes into the FAT array.

*Declaration:*  
void loadFAT(const char* disk)

*Parameters:*  
disk- the name of the FAT12 image being read.  

*Return Value:*  
loadFAT() has no return value.

### rootDirectory()
*Description:*  
The rootDirectory function is used to print out all of the file/subdirectory names from the root directory to the console.

*Declaration:*  
void rootDirectory(char* disk)

*Parameters:*  
disk- the name of the FAT12 image being read.  

*Return Value:*  
rootDirectory() has no return value.


### changeDirectory()
*Description:*  
The changeDirectory function prompts the user to enter the name of the directory that they would like to enter or "." to go to the previous directory. The entered directory name is used to search through the current directory to find the first logical cluster of the subdirectory.  If found, the subdirectory is loaded into the currrent directory and the previous directory is stored in the current directory struct. If "." is entered then the previous directory is loaded into the current directory.

*Declaration:*  
void changeDirectory(char* disk)

*Parameters:*  
disk- the name of the FAT12 image being read.  

*Return Value:*  
changeDirectory() has no return value.


### listDirectory()
*Description:*  
The listDirectory function prompts the user to enter "*" to list all the files in the current directory, "*.EXT" to list all files of a certain extension, or "FILENAME.EXT" to list a specific file.  The selected files will display their name, extension, file size, and first logical cluster to the console.

*Declaration:*  
void listDirectory()

*Parameters:*  
NONE

*Return Value:*  
listDirectory() has no return value.
