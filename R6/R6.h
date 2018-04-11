#ifndef _R6_
#define _R6_

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



#endif
