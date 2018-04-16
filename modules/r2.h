#ifndef MPX_SPRING2018_ABOVE_C_LEVEL_R2_H
#define MPX_SPRING2018_ABOVE_C_LEVEL_R2_H

int suspendPCB(int argc, char *argv[]);
int resumePCB(int argc, char *argv[]);
int setPriority(int argc, char *argv[]);
void showPCB(int argc, char* argv[]);
void showAll();
void showReady();
void showBlocked();

// Temporary
void CreatePCB(int argc, char *argv[], void* function);
void DeletePCB(int argc, char *argv[]);
int blockPCB(int argc, char *argv[]);
int unblockPCB(int argc, char *argv[]);

#endif