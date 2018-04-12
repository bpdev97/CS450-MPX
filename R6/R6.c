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
        printf("\n");
        if(argc == 2)
        {
            char chosenOption[1];
            printf("Please enter a number \n");
            printf("1) Print Boot Sector \n");
            printf("2) Print Root Directory \n");
            printf("3) Change Directory \n");
            printf("4) List Directory \n");
            printf("9) Exit \n");
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
            else if (chosenOption[0] == '9')
            {
                break;
            }
        }
        else if(argc == 3)
        {
            //print file
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
void loadInitialDirectory(char* disk)
{
    FILE* file = fopen(disk, "r");
    unsigned char filename[9];
    unsigned char extension[4];
    unsigned char filesize[5];
    unsigned char firstCluster[2];
    int validName = 1;
    int currentDirectoryEntry = 0;

     for(int i = 19; i <33; i++)
    {
        for(int j =0; j<16; j++)
        {
            memset(filename, 0, sizeof filename);
            validName = 1;
            fseek(file,(i*512)+(j*32), SEEK_SET);
            fread(filename, 8, 1, file);
            if(filename[0] == 0x00)
            {
                break;
            }
            for(int x =0; x<8;x++)
	    {
              if(filename[x] != ' ' && (filename[x] <= '0' || filename[x] >= 'Z'))
               {
                 validName = 0;
               }
               if(filename[x] == ' ')
               {
                  filename[x] = '\0';     
               }
            }
            if(validName)
            {
	        memset(extension, 0, sizeof extension);
	        fread(extension, 3, 1, file);
		fseek(file, 15, SEEK_CUR);
                fread(firstCluster, 2, 1, file);
		fread(filesize, 4, 1, file);
		filename[8] = '\0';
	        extension[3] = '\0';
		if(BytetoNumber(filesize, 4) != -1  && BytetoNumber(firstCluster, 2) >32)
		 {
		    strcpy(currentDirectory->fileNames[currentDirectoryEntry], filename);
                    strcpy(currentDirectory->fileExtension[currentDirectoryEntry], extension);
                    currentDirectory->fileSize[currentDirectoryEntry] = BytetoNumber(filesize, 4);
 		    currentDirectory->firstCluster[currentDirectoryEntry] =  BytetoNumber(firstCluster,2);
                    currentDirectoryEntry++;
                    currentDirectory->numberOfEntries = currentDirectoryEntry;
		 }
            }
       }
        if(filename[0] == 0x00)
         {
              break;
          }
    }
    currentDirectory->previousDir = NULL;
    fclose(file);

}
void loadFAT(const char* disk){
    FILE* file = fopen(disk, "r");
    unsigned char array[16];
    unsigned char fatDisk[3];

    fseek(file, 11, SEEK_SET);
    fread(array, 2, 1, file);

    bytesPerSector = BytetoNumber(array, 2);
    //printf("Bytes per Sector: %d\n",);

    fread(array, 1, 1, file);
    // printf("Sectors per Cluster: %d\n",BytetoNumber(array, 1));

    fread(array, 2, 1, file);
    //printf("Number of Reserved Sectors: %d\n",BytetoNumber(array, 2));

    fread(array, 1, 1, file);
    // printf("Number of FAT Copies: %d\n",BytetoNumber(array, 1));

    fread(array, 2, 1, file);
    // printf("Max Number of Root Directory Entries: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    //  printf("Total # of Sectors in the File System: %d\n",BytetoNumber(array, 2));

    fseek(file, 1, SEEK_CUR);
    fread(array, 2, 1, file);
    sectorsPerFat = BytetoNumber(array, 2);
    //printf("Number of Sectors per Fmalloc(sizeof *x)AT: %d\n",BytetoNumber(array, 2));

    float numberOfEntriesFloat = sectorsPerFat *bytesPerSector *((float)8/12);
    int numberOfEntries = (int) numberOfEntriesFloat;

    FAT = calloc(numberOfEntries, sizeof(int));
    fseek(file, 1*512, SEEK_SET);
    for(int i = 0; i <numberOfEntries; i= i+2)
    {
        fread(fatDisk, 3, 1, file);
        int firstByte = (int) fatDisk[0];
        int middleByte = (int) fatDisk[1];
        int lastByte = (int) fatDisk[2];
        //printf("\n %d", firstByte);
        //printf("\n %d", middleByte);
        // printf("\n %d", lastByte);
        int lsbNibble = (middleByte & 0x000F);
        int msbNibble = (middleByte & 0x00F0);
        lsbNibble = lsbNibble << 8;
        msbNibble = msbNibble >> 4;
        lastByte = lastByte << 4;

        FAT[i] = lsbNibble + firstByte;
        FAT[i+1] = msbNibble + lastByte;
        //printf("\n %d", FAT[i]);
        // printf("\n %d", FAT[i+1]);
    }

    fclose(file);
}

void BootSector(char* disk){
    FILE* file = fopen(disk, "r");
    unsigned char array[16];

    fseek(file, 11, SEEK_SET);
    fread(array, 2, 1, file);
    array[10] = '\0';

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

    fseek(file, 1, SEEK_CUR);
    fread(array, 2, 1, file);
    printf("Number of Sectors per FAT: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    printf("Sectors per Track: %d\n",BytetoNumber(array, 2));

    fread(array, 2, 1, file);
    printf("Number of Heads: %d\n",BytetoNumber(array, 2));

    fread(array, 4, 1, file);
    fread(array, 4, 1, file);
    printf("Total Sector Count for FAT32: %d\n",BytetoNumber(array, 4));

    fseek(file, 2, SEEK_CUR);
    fread(array, 1, 1, file);
    printf("Boot Signature: %x\n",BytetoNumber(array, 1));

    fread(array, 4, 1, file);
    printf("Volume ID: %d\n",BytetoNumber(array, 4));

    memset(array, 0, 16);
    fread(array, 11, 1, file);
    printf("Volume Label: %s\n",array);
    memset(array, 0, 16);

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

     for(int i = 19; i <33; i++)
    {
        for(int j =0; j<16; j++)
        {
            memset(filename, 0, sizeof filename);
            validName = 1;
            fseek(file,(i*512)+(j*32), SEEK_SET);
            fread(filename, 8, 1, file);
            if(filename[0] == 0x00)
            {
                break;
            }
            for(int x =0; x<8;x++)
	    {
              if(filename[x] != ' ' && (filename[x] <= '0' || filename[x] >= 'Z'))
               {
                 validName = 0;
               }
               if(filename[x] == ' ')
               {
                  filename[x] = '\0';     
               }
            }
            if(validName)
            {
	        memset(extension, 0, sizeof extension);
	        fread(extension, 3, 1, file);
		fseek(file, 15, SEEK_CUR);
                fread(firstCluster, 2, 1, file);
		fread(filesize, 4, 1, file);
		filename[8] = '\0';
	        extension[3] = '\0';
		if(BytetoNumber(filesize, 4) != -1  && BytetoNumber(firstCluster, 2) >32)
		 {
		    printf("FileName: %s",filename);
		    printf(".%s\n",extension);
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
}
void listDirectory(char* disk)
{
//ask them to specify what they want listed 
     for(int i =0; i<currentDirectory->numberOfEntries; i++)
     {
      printf("FileName: %s",currentDirectory->fileNames[i]);
      printf(".%s",currentDirectory->fileExtension[i]);
      printf(" FileSize: %d",currentDirectory->fileSize[i]);
      printf(" FirstCluster:%d\n\n",currentDirectory->firstCluster[i]);
     }
    
}

void testFileAttributes(char* disk)
{
    FILE* file = fopen(disk, "r");
    unsigned char array[8];

    fseek(file,(20*512) + (7*32), SEEK_SET);
    fread(array, 8, 1, file);
    printf("FileName: %s",array);
    memset(array, 0, sizeof array);
    fread(array, 3, 1, file);
    printf(". %s \n",array);

    memset(array, 0, sizeof array);
    fread(array, 1, 1, file);
    printf("Attributes:");
    if((BytetoNumber(array, 1) & 0x01) > 0x00)
    {
        printf("Read-Only, ");
    }
    if((BytetoNumber(array, 1) & 0x02) > 0x00)
    {
        printf("Hidden, ");
    }
    if((BytetoNumber(array, 1) & 0x04) > 0x00)
    {
        printf("System, ");
    }
    if((BytetoNumber(array, 1) & 0x10) > 0x00)
    {
        printf("Subdirectory ");
    }
    printf("\n");

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("Reserved: %d\n",BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("CreationTime:");
    returnTime(BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);

    printf("CreationDate: ");
    returnDate(BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("Last Access Date: ");
    returnDate(BytetoNumber(array, 2));

    fseek(file, 2, SEEK_CUR);
    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("Last Write Time:");
    returnTime(BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("Last Write Date: ");
    returnDate(BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 2, 1, file);
    printf("First Logic Cluster: %d\n",BytetoNumber(array, 2));

    memset(array, 0, sizeof array);
    fread(array, 4, 1, file);
    printf("File Size: %d\n",BytetoNumber(array, 4));

    fclose(file);
}


void returnDate(int date)
{
    int day = (date & 0x001F);
    date = date>> 5;
    int month = (date & 0x00F);
    date = date>> 4;
    int year = (date & 0x7F) + 1980;
    char dayStr[3];
    char monthStr[3];
    char yearStr[5];
    sprintf(dayStr, "%d", day);
    sprintf(monthStr, "%d", month);
    sprintf(yearStr, "%d", year);
    printf("%s%s", monthStr, "/");
    printf("%s%s", dayStr, "/");
    printf("%s%s", yearStr, "\n");

}
void returnTime(int time)
{
    int second = (time & 0x001F);
    time = time>> 5;
    int minute = (time & 0x03F);
    time = time>> 6;
    int hour = (time & 0x1F);
    char secondStr[2];
    char minuteStr[2];
    char hourStr[4];
    sprintf(secondStr, "%d", second);
    sprintf(minuteStr, "%d", minute);
    sprintf(hourStr, "%d", hour);
    printf("%s%s", hourStr, ":");
    printf("%s%s", minuteStr, ":");
    printf("%s%s", secondStr, "\n");
}
