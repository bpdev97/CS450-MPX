
#ifndef MPX_SPRING2018_ABOVE_C_LEVEL_R1_H
#define MPX_SPRING2018_ABOVE_C_LEVEL_R1_H

void help (int numword, char* args[]);
void version ();
void shutdown (int argc, char* argv[]);
void settime (int argc, char* argv[]);
void setdate (int argc, char* argv[]);
void getdate ();
void gettime ();
int tobcd (int binary);
void printbcd (int bcd);

#endif
