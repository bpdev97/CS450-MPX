
#ifndef MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
#define MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H

void help (int numword, char* args[]);
void shutdown (int argc, char* argv[]);
void settime (int argc, char* argv[]);
void setdate (int argc, char* argv[]);
void getdate (int argc, char* argv[]);
void gettime (int argc, char* argv[]);
int tobcd (int binary);

#endif //MPX_SPRING2018_ABOVE_C_LEVEL_PROCESSCOMMAND_H
