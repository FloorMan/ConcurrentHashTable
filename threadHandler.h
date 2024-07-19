#ifndef THREADHANDLER_H
#define THREADHANDLER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "parseCommands.h"

  
  
  
  
  //Create an arrays of threads (needed to be fixed accordingly reading input from files)
int numberThreads;
pthread_t threads[];
struct command_t cmds[];


void* handleCommand(void* arg)



#endif 