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

### BytetoNumber()
*Description:*  
The BytetoNumber function converts a byte into a number. This was implemented to satisfy the little-endian style and to easily convert values.

*Declaration:*  
int BytetoNumber(unsigned char *array, int length)

*Parameters:*  
array-  Used to store the values.
length- Used to keep track of the length of the byte. 

*Return Value:*  
BytetoNumber() returns the int value.

### NumbertoByte()
*Description:*  
The NumbertoByte function converts a number into a byte. This was implemented to satisfy the little-endian style and to easily convert values.

*Declaration:*  
void NumbertoByte(int number, unsigned char *array, int length)

*Parameters:*  
number- Used to keep track of the number being converted.
array-  Used to store the values.
length- Used to keep track of the length of the byte.

*Return Value:*  
NumbertoByte() has no return value.

### BootSector()
*Description:*  
The BootSector function is used to print out the bootsector table with all of the file values to the console.

*Declaration:*  
void BootSector(char* disk)

*Parameters:*  
disk- tells where the disk image file is located.

*Return Value:*  
BootSector() has no return value.

### renameFile()
*Description:*  
The renameFile function is used to allow the user to rename the file and load it back into the directory.

*Declaration:*  
void renameFile (char* disk, char* originalfilename, char* originalfileextension, char* filename, char* fileExtension)

*Parameters:*  
disk- tells where the disk image file is located.
originalfilename- the original name of the file.
originalfileextension- the original extension of the file.
filename- the new name of the file.
fileextension- the new extension of the file.

*Return Value:*  
renameFile() has no return value.

### type()
*Description:*  
The renameFile function is used to print the contents of any file with the extension "TXT", "BAT", or "C".

*Declaration:*  
void type(char* disk, char* filename, char* fileExtension)

*Parameters:*  
disk- tells where the disk image file is located.
filename- the name of the file.
fileextension- the extension of the file.

*Return Value:*  
type() has no return value.


