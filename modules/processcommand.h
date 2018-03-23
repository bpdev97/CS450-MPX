
#ifndef MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
#define MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H

void help (int numword, char* args[]);
void version ();
void shutdown (int argc, char* argv[]);
void settime (int argc, char* argv[]);
void setdate (int argc, char* argv[]);
void getdate ();
void gettime ();
char* returnCurrentTime();
char* returnCurrentYear();
char* returnCurrentMonth();
char* returnCurrentDay();
char* returnBCDString (int bcd);
int tobcd (int binary);
//int frombcd (int bcd);
void printbcd (int bcd);
void showPCB(int argc, char* argv[]);
void showReady();
void showBlocked();
void showAll();
void CreatePCB(int argc, char *argv[], void* function);
void DeletePCB(int argc, char *argv[]);
int blockPCB(int argc, char *argv[]);
int unblockPCB(int argc, char *argv[]);
int suspendPCB(int argc, char *argv[]);
int resumePCB(int argc, char *argv[]);
int setPriority(int argc, char *argv[]);
void createAlarm(int argc, char *argv[]);

#endif //MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
