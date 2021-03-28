#include "taxpayer.h"
#include "coord.h"
#include <unistd.h> 
#include <sys/wait.h>


//use ./myhie -i 1batch-1000.csv -k 3 -r -a 0 -o a -s OutputFile

#ifndef MYHIE
#define MYHIE

void root(pid_t pid, timeval tv1);       //root node code
void signalHandler(int signal);     //SIGUSR1 handler
void signalHandler2(int signal);    //SIGUSR2 handler

#endif