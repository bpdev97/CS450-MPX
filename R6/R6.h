#ifndef _R6_
#define _R6_


typedef struct directory{
char fileNames[100][9];
char fileExtension[100][4];
int fileSize[100];
int firstCluster[100];
int numberOfEntries;
struct directory* previousDir;
} DIRECTORY;


int main (int argc, char *argv[]);
int BytetoNumber(unsigned char* array, int length);
void NumbertoByte(int number, unsigned char* array, int length);
void BootSector(char* disk);
void rootDirectory(char* disk);
void returnTime(int time);
void returnDate(int date);
void listDirectory(char* disk);
void changeDirectory(char* disk);
void loadFAT(const char* disk);
void loadInitialDirectory(char* disk);
void testFileAttributes(char* disk);



#endif
