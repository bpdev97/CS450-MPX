#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "R6.h"


int sectorsPerFat;
int bytesPerSector;
int* FAT;
DIRECTORY* currentDirectory;

int main (int argc, char *argv[])
{
    currentDirectory = malloc(sizeof(DIRECTORY)); 
    loadFAT(argv[1]); 
    loadInitialDirectory(argv[1]);
    while(1)
    {
        if(argc != 3) {
            printf("\n");
        }
        if(argc == 2)
        {
            char chosenOption[1];
            printf("Please enter a number \n");
            printf("1) Print Boot Sector \n");
            printf("2) Print Root Directory \n");
            printf("3) Change Directory \n");
            printf("4) List Directory \n");
            printf("5) Type \n");
            printf("6) Rename \n");
            printf("7) Exit \n");
            scanf("%s", chosenOption);
            if(chosenOption[0] == '1')
            {
                BootSector(argv[1]);
            }
            else if (chosenOption[0] == '2')
            {
                rootDirectory(argv[1]);
            }
            else if (chosenOption[0] == '3')
            {
                changeDirectory(argv[1]);
            }
            else if (chosenOption[0] == '4')
            {
                listDirectory(argv[1]);
            }
            else if (chosenOption[0] == '5')
            {
                char fileName[50];
                char fileExtension[50];
                //scans for file name and file extension
		printf("Please enter FILENAME without an extension \n");
                scanf("%s", fileName);
                printf("Please enter EXTENSION without a '.' \n");
                scanf("%s", fileExtension);
                //determines user input
                type(argv[1], fileName, fileExtension, 1);
            }
            else if (chosenOption[0] == '6')
            {
                char originalfilename[50];
                char originalfileextension[50];
                char fileName[50];
                char fileExtension[50];
                //scans for file name and file extension
                printf("Please enter ORIGINAL FILENAME without an extension \n");
                scanf("%s", originalfileextension); // you flipped variables for originalfileextension and originalfilename
                printf("Please enter ORIGINAL EXTENSION without a '.' \n");
                scanf("%s", originalfilename);
                printf("Please enter NEW FILENAME without an extension \n");
                scanf("%s", fileName);
                printf("Please enter NEW EXTENSION without a '.' \n");
                scanf("%s", fileExtension);
                //determines user input
                renameFile(argv[1], originalfileextension, originalfilename, fileName, fileExtension);
            }
            else if (chosenOption[0] == '7')
            {
                break;
            }
        }
        else if(argc == 3)
        {
            argument(argv[1], argv[2]);
            return 0;
        }
        else
        {
            printf("Argument error \n");
        }
    }
}
//Little-endian style
int BytetoNumber(unsigned char *array, int length) {
    int value = 0;
    for (int i = 0; length > i; i++) {
        value = value + (array[i] << (i * 8));
    }
    return value;
}
//Little-endian style
void NumbertoByte(int number, unsigned char *array, int length) {
    for (int i = 0; length > i; i++) {
        array[i] = (number & (255 << (i * 8))) >> (i * 8);
    }
}
void loadInitialDirectory(char* disk)//loads root directory to current directory
{
    FILE* file = fopen(disk, "r");
    unsigned char filename[9];
    unsigned char extension[4];
    unsigned char filesize[5];
    unsigned char firstCluster[2];
    int validName = 1;
    int currentDirectoryEntry = 0;

    for(int i = 19; i <33; i++) // Root clusters is btw 19 and 32
    {
        for(int j =0; j<16; j++) // 32 bit file entries, cluster is 512 bits, 512/32 = 16
        {
            memset(filename, 0, sizeof filename);
            validName = 1;
            fseek(file,(i*512)+(j*32), SEEK_SET);//seeks to each file entry in root directory
            fread(filename, 8, 1, file);//scans file name

            if(filename[0] == 0x00) //break out of loop if 0 is found, means no more files are left
            {
                break;
            }
            for(int x =0; x<8;x++)//filename max size is 8
            {
                if(filename[x] != ' ' && (filename[x] < '0' || filename[x] > 'Z')) // filename must be only capital letters or numbers
                {
                    validName = 0;
                }
                if(filename[x] == ' ')//replaces empty chars with string terminators
                {
                    filename[x] = '\0';
                }
            }
            if(validName)// if valid file name, reads image and stores root directory file in current directory
            {
                memset(extension, 0, sizeof extension);
                fread(extension, 3, 1, file); //reads extnesion from image
                fseek(file, 15, SEEK_CUR);
                fread(firstCluster, 2, 1, file); //reads first cluster
                fread(filesize, 4, 1, file); // reads file size
                filename[8] = '\0';//adds null terminator
                extension[3] = '\0';//adds null terminator

                if(BytetoNumber(filesize, 4) != -1  && BytetoNumber(firstCluster, 2) >32)//must be a valid file size and non reserved cluster
                {
                    strcpy(currentDirectory->fileNames[currentDirectoryEntry], filename);//stores filename in current directory
                    if(extension[0] == ' ')//Sub directories dont have extensions so I made subdirectory extensions 'DIR'
                    {
                        strcpy(currentDirectory->fileExtension[currentDirectoryEntry], "DIR");
                    }
                    else
                    {
                        strcpy(currentDirectory->fileExtension[currentDirectoryEntry], extension);//stores file extension in current directory
                    }

                    currentDirectory->fileSize[currentDirectoryEntry] = BytetoNumber(filesize, 4); // stores file size in current directory
                    currentDirectory->firstCluster[currentDirectoryEntry] =  BytetoNumber(firstCluster,2);// stores first cluster in current directory
                    currentDirectoryEntry++;
                    currentDirectory->numberOfEntries = currentDirectoryEntry;//increases number of file entries in current directory
                }
            }
        }
        if(filename[0] == 0x00)
        {
            break;
        }
    }
    currentDirectory->previousDir = NULL;//root directory does not have previous directory
    fclose(file);

}
void loadFAT(const char* disk){
    FILE* file = fopen(disk, "r");
    unsigned char array[16]; //just a buffer
    unsigned char fatDisk[3]; //stores 3 bytes from FAT table that is converted to 2 12bit integers

    //searches through boot sector to find bytesPerSector and sectorsPerFat, needed for FAT int array initialization
    fseek(file, 11, SEEK_SET);
    fread(array, 2, 1, file);

    bytesPerSector = BytetoNumber(array, 2);
    
    fseek(file, 9, SEEK_CUR);
    fread(array, 2, 1, file);

    sectorsPerFat = BytetoNumber(array, 2);

    float numberOfEntriesFloat = sectorsPerFat *bytesPerSector *((float)8/12); // uses sectorsPerFat and bytesPerSector to calculate max number of fat int array entries
    int numberOfEntries = (int) numberOfEntriesFloat;

    FAT = calloc(numberOfEntries, sizeof(int));// allocates memory to FAT int array
    fseek(file, 1*512, SEEK_SET);
    for(int i = 0; i <numberOfEntries; i= i+2) //converts Fat table to FAT int array, 3 bytes from the Fat table into 2 12bit ints in the FAT array using little endian style,
    {
        fread(fatDisk, 3, 1, file);
        int firstByte = (int) fatDisk[0];
        int middleByte = (int) fatDisk[1];
        int lastByte = (int) fatDisk[2];
        int lsbNibble = (middleByte & 0x000F);
        int msbNibble = (middleByte & 0x00F0);
        lsbNibble = lsbNibble << 8;
        msbNibble = msbNibble >> 4;
        lastByte = lastByte << 4;

        FAT[i] = lsbNibble + firstByte;
        FAT[i+1] = msbNibble + lastByte;
    }

    fclose(file);
}

void BootSector(char* disk){
    //reading contents of file
    FILE* file = fopen(disk, "r");
    unsigned char array[16];

    fseek(file, 11, SEEK_SET);
    fread(array, 2, 1, file);
    array[10] = '\0';

    //displays file values by converting byte to number
    printf("Bytes per Sector: %d\n",BytetoNumber(array, 2));

    fread(array, 1, 1, file);
    printf("Sectors per Cluster: %d\n",BytetoNumber(array, 1));

    fread(array, 2, 1, file);
    printf("Number of Reserved Sectors: %d\n",BytetoNumber(array, 2));

    fread(array, 1, 1, file);
    printf("Number of FAT Copies: %d\n",BytetoNumber(array, 1));

    fread(array, 2, 1, file);
    printf("Max Number of Root Directory Entries: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    printf("Total # of Sectors in the File System: %d\n",BytetoNumber(array, 2));

    //type IGNORE
    fseek(file, 1, SEEK_CUR);
    fread(array, 2, 1, file);
    printf("Number of Sectors per FAT: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    printf("Sectors per Track: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    printf("Number of Heads: %d\n",BytetoNumber(array, 2));

    //type IGNORE
    fread(array, 4, 1, file);
    fread(array, 4, 1, file);
    printf("Total Sector Count for FAT32: %d\n",BytetoNumber(array, 4));

    //type IGNORE
    fseek(file, 2, SEEK_CUR);
    fread(array, 1, 1, file);
    printf("Boot Signature: %x\n",BytetoNumber(array, 1));

    fread(array, 4, 1, file);
    printf("Volume ID: %d\n",BytetoNumber(array, 4));

    //type IGNORE
    memset(array, 0, 16);
    fread(array, 11, 1, file);
    printf("Volume Label: %s\n",array);
    //clears out array
    memset(array, 0, 16);

    //type IGNORE
    fread(array, 8, 1, file);
    printf("File System Type: %s\n",array);
    memset(array, 0, 16);
    fclose(file);


}
void rootDirectory(char* disk)
{
    FILE* file = fopen(disk, "r");
    unsigned char filename[9];
    unsigned char extension[4];
    unsigned char filesize[5];
    unsigned char firstCluster[2];
    int validName = 1;

    for(int i = 19; i <33; i++)//Root directory clusters 19-32
    {
        for(int j =0; j<16; j++)//16 file entries
        {
            memset(filename, 0, sizeof filename);
            validName = 1;
            fseek(file,(i*512)+(j*32), SEEK_SET);//finds file entry
            fread(filename, 8, 1, file);
            if(filename[0] == 0x00)// if 0 found, means no more files, break out of loop
            {
                break;
            }
            for(int x =0; x<8;x++)
            {
                if(filename[x] != ' ' && (filename[x] < '0' || filename[x] > 'Z'))// valid names have only capital letters and numbers
                {
                    validName = 0;
                }
                if(filename[x] == ' ') //changes blank chars to string terminators
                {
                    filename[x] = '\0';
                }
            }
            if(validName)//if valid name, root directory filname and extension are printed
            {
                memset(extension, 0, sizeof extension);
                fread(extension, 3, 1, file);
                fseek(file, 15, SEEK_CUR);
                fread(firstCluster, 2, 1, file);
                fread(filesize, 4, 1, file);
                filename[8] = '\0';
                extension[3] = '\0';
                if(BytetoNumber(filesize, 4) != -1  && BytetoNumber(firstCluster, 2) >32)//must be valid filesize and nonreserved cluster
                {
                    printf("FileName: %s",filename);
                    if(extension[0] == ' ')//subdirectories dont have extensions so it prints "DIR" instead
                    {
                        printf(".%s\n","DIR");
                    }
                    else
                    {
                        printf(".%s\n",extension);
                    }
                }
            }
        }
        if(filename[0] == 0x00)
        {
            break;
        }
    }
    fclose(file);
}
void changeDirectory(char* disk)
{
    char input[13];
    int nextDirectoryCluster =0;
    printf("Enter DIRECTORYNAME.DIR or . to go back to previous directory\n"); //prompts user and gets sub directory name
    scanf("%s", input); 
    if(input[0] == '.'  && input[1] == '\0') //loads previous directory if '.' is entered
    {
        if(currentDirectory->previousDir == NULL)
        {
            printf("Already in root Directory\n");
        }
        else
        {
            currentDirectory = currentDirectory->previousDir;
            printf("Previous Directory Loaded\n");
        }
    }
    else if(input[0] >= 'A' && input[0] <= 'Z'  || input[0] >= '0' && input[0] <= '9' )//checks user input for valid subdirectory name
    {
        int directoryFound =0;
        char filename[9];
        char extension[4];
        for(int j =0; j<8; j++) //parses file name from user input
        {
            if(input[j] == '.')
            {
                filename[j] = '\0';
                break;
            }
            else
            {
                filename[j] = input[j];
            }
        }
        int dotFound = 0;
        for(int j =0; j<13; j++) //parses extension from user input
        {
            if(input[j] == '.')
            {
                dotFound = 1;
            }
            if(dotFound)
            {
                extension[0] = input[j+1];
                extension[1] = input[j+2];
                extension[2] = input[j+3];
                extension[3] = '\0';
                break;
            }
        }

        for(int i =0; i<currentDirectory->numberOfEntries; i++)//searches current directory for subdirectory
        {
            if(strcmp(filename, currentDirectory->fileNames[i]) == 0 && strcmp(extension, currentDirectory->fileExtension[i]) == 0 && strcmp(extension, "DIR") == 0)
            {
                directoryFound =1;
                nextDirectoryCluster = currentDirectory->firstCluster[i];
            }
        }
        if(!directoryFound)
        {
            printf("Subdirectory not found\n");
        }
    }
    else
    {
        printf("Invalid Entry \n");
    }

    if(nextDirectoryCluster !=0) // means subdirectory found, and will load subdirectory to current directory
    {
        FILE* file = fopen(disk, "r");
        DIRECTORY* subDirectory = malloc(sizeof(DIRECTORY));
        subDirectory->previousDir = currentDirectory; //stores current directory as previous directory
        unsigned char filename[9];
        unsigned char extension[4];
        unsigned char filesize[5];
        unsigned char firstCluster[2];
        int validName = 1;
        int currentDirectoryEntry = 0;
        for(int j =0; j<16; j++) //searches through subdirectory  data for file entries
        {
            memset(filename, 0, sizeof filename);
            validName = 1;
            fseek(file,((nextDirectoryCluster+31)*512)+(j*32), SEEK_SET);//finds subdirectory file entries in data clusters
            fread(filename, 8, 1, file);

            if(filename[0] == 0x00)// 0 means no files left in cluster
            {
                break;
            }
            for(int x =0; x<8;x++)
            {
                if(filename[x] != ' ' && (filename[x] < '0' || filename[x] > 'Z'))//checks for valid file name
                {
                    validName = 0;
                }
                if(filename[x] == ' ')//replaces empty char with string terminator
                {
                    filename[x] = '\0';
                }
            }
            if(validName)//if valid name, file entry is stored in current directory
            {
                memset(extension, 0, sizeof extension);
                fread(extension, 3, 1, file);
                fseek(file, 15, SEEK_CUR);
                fread(firstCluster, 2, 1, file);
                fread(filesize, 4, 1, file);
                filename[8] = '\0';
                extension[3] = '\0';
                if(BytetoNumber(filesize, 4) != -1  && BytetoNumber(firstCluster, 2) >32) // must be valid file size and nonreserved cluster
                {
                    strcpy(subDirectory->fileNames[currentDirectoryEntry], filename);
                    if(extension[0] == ' ')//subdirectories do not have extension names, so I add one
                    {
                        strcpy(subDirectory->fileExtension[currentDirectoryEntry], "DIR");
                    }
                    else
                    {
                        strcpy(subDirectory->fileExtension[currentDirectoryEntry], extension);
                    }

                    subDirectory->fileSize[currentDirectoryEntry] = BytetoNumber(filesize, 4);
                    subDirectory->firstCluster[currentDirectoryEntry] =  BytetoNumber(firstCluster,2);
                    currentDirectoryEntry++;
                    subDirectory->numberOfEntries = currentDirectoryEntry;
                }
            }

            if(filename[0] == 0x00)
            {
                break;
            }
        }
        currentDirectory = subDirectory;//makes subdirectory the current directory

        fclose(file);
    }
}
void listDirectory()
{
    char input[13];
    printf("Enter * to list all files, FILENAME.EXT for specific file, or *.EXT for files of a certain type \n");//prompts user
    scanf("%s", input);
    if(input[0] == '*'  && input[1] == '\0') //if input is '*' all files in current directory are printed
    {
        for(int i =0; i<currentDirectory->numberOfEntries; i++)
        {
            printf("FileName: %s",currentDirectory->fileNames[i]);
            printf(".%s",currentDirectory->fileExtension[i]);
            printf(" FileSize: %d",currentDirectory->fileSize[i]);
            printf(" FirstCluster:%d\n\n",currentDirectory->firstCluster[i]);
        }
    }
    else if(input[0] == '*'  && input[1] == '.'  && input[5] == '\0')// if input is '*.EXT' all files of a certain extension in the current directory are printed
    {
        int filesFound =0;
        char extension[4];
        extension[0] = input[2];
        extension[1] = input[3];
        extension[2] = input[4];
        for(int i =0; i<currentDirectory->numberOfEntries; i++)
        {
            if(strcmp(extension, currentDirectory->fileExtension[i]) == 0) //searches current directory for extension type and then prints appropriate files 
            {
                filesFound =1;
                printf("FileName: %s",currentDirectory->fileNames[i]);
                printf(".%s",currentDirectory->fileExtension[i]);
                printf(" FileSize: %d",currentDirectory->fileSize[i]);
                printf(" FirstCluster:%d\n\n",currentDirectory->firstCluster[i]);
            }
        }
        if(!filesFound)
        {
            printf("No Files Found with that extension \n");
        }
    }
    else if(input[0] >= 'A' && input[0] <= 'Z'  || input[0] >= '0' && input[0] <= '9' )// if input is "FILENAME.EXT", checks for valid name
    {
        int filesFound =0;
        char filename[9];
        char extension[4];
        for(int j =0; j<8; j++)//parse filename
        {
            if(input[j] == '.')
            {
                filename[j] = '\0';
                break;
            }
            else
            {
                filename[j] = input[j];
            }
        }
        int dotFound = 0;
        for(int j =0; j<13; j++) //parse extension
        {
            if(input[j] == '.')
            {
                dotFound = 1;
            }
            if(dotFound)
            {
                extension[0] = input[j+1];
                extension[1] = input[j+2];
                extension[2] = input[j+3];
                extension[3] = '\0';
                break;
            }
        }

        for(int i =0; i<currentDirectory->numberOfEntries; i++) //searches current directory for file name and prints file info
        {
            if(strcmp(filename, currentDirectory->fileNames[i]) == 0 && strcmp(extension, currentDirectory->fileExtension[i]) == 0)
            {
                filesFound =1;
                printf("FileName: %s",currentDirectory->fileNames[i]);
                printf(".%s",currentDirectory->fileExtension[i]);
                printf(" FileSize: %d",currentDirectory->fileSize[i]);
                printf(" FirstCluster:%d\n\n",currentDirectory->firstCluster[i]);
            }
        }
        if(!filesFound)
        {
            printf("File not found\n");
        }
    }
    else
    {
        printf("Invalid Entry \n");
    }
}
void renameFile (char* disk, char* originalfilename, char* originalfileextension, char* filename, char* fileExtension){
    printf("Searching for file %s.%s\n", originalfilename, originalfileextension);
    //opening file and initializing
    FILE* file = fopen(disk, "r+b");
    fseek(file, 19 * 512, SEEK_SET);
    char nameBuffer [9];
    char extensionBuffer [4];
    for(int i = 0; i < (14*16) ; i++){
        fread(nameBuffer, 1,8 , file);
        nameBuffer[8] = 0;
        fread(extensionBuffer, 1,3 , file);
        extensionBuffer[3] = 0;
        //locating file, file name can have a max of 8 characters
        if(strncmp(nameBuffer, originalfilename, strlen(originalfilename)) == 0 && strncmp(extensionBuffer, originalfileextension, strlen(originalfileextension)) == 0){
            printf("Found file %s.%s\n", nameBuffer, extensionBuffer);
            fseek(file, -11, SEEK_CUR);
            char buffer[8];
            memset(buffer, ' ', 8);
            strcpy(buffer, filename);
            buffer[strlen(buffer)] = ' ';
          /*  for(int j = 0; j < 8; j++){
                printf("%X ", buffer[j]);
            }*/
            printf("\n");
            fwrite(buffer, 1, 8, file);
            memset(buffer, ' ', 8);
            strcpy(buffer, fileExtension);
            buffer[strlen(buffer)] = ' ';
            fwrite(buffer, 1, 3, file);
            fclose(file);
            loadInitialDirectory(disk);
            return;
        }else{
            fseek(file, 21, SEEK_CUR);
        }
    }
    fclose(file);
}

void type(char* disk, char* filename, char* fileExtension, int page){
    int size = 0;
    int cluster = 0;
    //search struct for filename, obtain size and cluster
    for(int i = 0; i < currentDirectory->numberOfEntries; i++){
        if(strncmp(filename, currentDirectory->fileNames[i], 8) == 0 && strncmp(fileExtension, currentDirectory->fileExtension[i], 3) == 0){
            size = currentDirectory->fileSize[i];
            cluster = currentDirectory->firstCluster[i];
            break;
        }
    }
    //set buffer to 513 (take into account NULL)
    char buffer[513];
    FILE* file = fopen(disk, "r");
    while(size > 0){
        if(page) {
            printf("\n(press any key to continue, q to quit)\n");
            if (getchar() == 'q') {
                break;
            }
        }

        //printing out the contents of the file, must add 31 to get physical cluster
        fseek(file, 512 * (cluster + 31), SEEK_SET);
        fread(buffer, 1, 512, file);
        if(size > 512){
            buffer[512] = 0;
        }
        else{
            buffer[size] = 0;
        }
        fwrite(buffer, 1, size > 512 ? 512 : size, stdout);
        size = size - 512;
        //obtained from FAT table
        cluster = FAT[cluster];
    }
    if(page) {
        printf("\n");
    }
    fclose(file);
}

void argument(char* disk, char* thefilename){
    char* directory = strtok(thefilename, "/");
    thefilename = strtok(NULL, "/");
    if(thefilename == NULL){
        thefilename = directory;
        directory = NULL;
    }
    FILE *file = fopen(disk, "r");
    if(directory != NULL) {
        char input[13];
        int nextDirectoryCluster = 0;
        strcpy(input, directory);

        if (input[0] >= 'A' && input[0] <= 'Z' ||
                   input[0] >= '0' && input[0] <= '9')//checks user input for valid subdirectory name
        {
            int directoryFound = 0;
            char filename[9];
            char extension[4];
            for (int j = 0; j < 8; j++) //parses file name from user input
            {
                if (input[j] == '.') {
                    filename[j] = '\0';
                    break;
                } else {
                    filename[j] = input[j];
                }
            }
            int dotFound = 0;
            for (int j = 0; j < 13; j++) //parses extension from user input
            {
                if (input[j] == '.') {
                    dotFound = 1;
                }
                if (dotFound) {
                    extension[0] = input[j + 1];
                    extension[1] = input[j + 2];
                    extension[2] = input[j + 3];
                    extension[3] = '\0';
                    break;
                }
            }

            for (int i = 0; i < currentDirectory->numberOfEntries; i++)//searches current directory for subdirectory
            {
                if (strcmp(filename, currentDirectory->fileNames[i]) == 0 &&
                    strcmp(extension, currentDirectory->fileExtension[i]) == 0 && strcmp(extension, "DIR") == 0) {
                    directoryFound = 1;
                    nextDirectoryCluster = currentDirectory->firstCluster[i];
                    // printf("SubDirectory Cluster: %d\n",nextDirectoryCluster);
                }
            }
            if (!directoryFound) {
                printf("Subdirectory not found\n");
                return;
            }
        } else {
            printf("Invalid Entry \n");
            return;
        }

        if (nextDirectoryCluster != 0) // means subdirectory found, and will load subdirectory to current directory
        {

            DIRECTORY *subDirectory = malloc(sizeof(DIRECTORY));
            subDirectory->previousDir = currentDirectory; //stores current directory as previous directory
            unsigned char filename[9];
            unsigned char extension[4];
            unsigned char filesize[5];
            unsigned char firstCluster[2];
            int validName = 1;
            int currentDirectoryEntry = 0;
            for (int j = 0; j < 16; j++) //searches through subdirectory  data for file entries
            {
                memset(filename, 0, sizeof filename);
                validName = 1;
                fseek(file, ((nextDirectoryCluster + 31) * 512) + (j * 32), SEEK_SET);
                fread(filename, 8, 1, file);

                if (filename[0] == 0x00)// 0 means no files left in cluster
                {
                    break;
                }
                for (int x = 0; x < 8; x++) {
                    if (filename[x] != ' ' && (filename[x] < '0' || filename[x] > 'Z'))//checks for valid file name
                    {
                        validName = 0;
                    }
                    if (filename[x] == ' ')//replaces empty char with string terminator
                    {
                        filename[x] = '\0';
                    }
                }
                if (validName)//if valid name, file entry is stored in current directory
                {
                    memset(extension, 0, sizeof extension);
                    fread(extension, 3, 1, file);
                    fseek(file, 15, SEEK_CUR);
                    fread(firstCluster, 2, 1, file);
                    fread(filesize, 4, 1, file);
                    filename[8] = '\0';
                    extension[3] = '\0';
                    if (BytetoNumber(filesize, 4) != -1 &&
                        BytetoNumber(firstCluster, 2) > 32) // must be valid file size and nonreserved cluster
                    {
                        strcpy(subDirectory->fileNames[currentDirectoryEntry], filename);
                        if (extension[0] == ' ')//subdirectories do not have extension names, so I add one
                        {
                            strcpy(subDirectory->fileExtension[currentDirectoryEntry], "DIR");
                        } else {
                            strcpy(subDirectory->fileExtension[currentDirectoryEntry], extension);
                        }

                        subDirectory->fileSize[currentDirectoryEntry] = BytetoNumber(filesize, 4);
                        subDirectory->firstCluster[currentDirectoryEntry] = BytetoNumber(firstCluster, 2);
                        currentDirectoryEntry++;
                        subDirectory->numberOfEntries = currentDirectoryEntry;
                    }
                }

                if (filename[0] == 0x00) {
                    break;
                }
            }
            currentDirectory = subDirectory;//makes subdirectory the current directory

        }
    }

    fclose(file);
    thefilename = strtok(thefilename, ".");
    char* ext = strtok(NULL, ".");
    // printf("Filename: %s, Ext: %s\n", thefilename, ext);
    type(disk, thefilename, ext, 0);
}



