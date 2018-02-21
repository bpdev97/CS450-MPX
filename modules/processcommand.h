
#ifndef MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
#define MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H

void help (int numword, char* args[]);
void version (int argc, char* argv[]);
void shutdown (int argc, char* argv[]);
void settime (int argc, char* argv[]);
void setdate (int argc, char* argv[]);
void getdate (int argc, char* argv[]);
void gettime (int argc, char* argv[]);
int tobcd (int binary);
//int frombcd (int bcd);
void printbcd (int bcd);
void showPCB(int argc, char* argv[]);
void showReady();
void showBlocked();
void showAll();
void CreatePCB(int argc, char *argv[]);
void DeletePCB(int argc, char *argv[]);

#endif //MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
