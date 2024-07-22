#ifndef THREADHANDLER_H
#define THREADHANDLER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "parseCommands.h"
#include "chash.h"

  //Create an arrays of threads (needed to be fixed accordingly reading input from files)
int numThreads;
pthread_t threads[];
struct command_t cmds[];
// Needs head of hashtable to call commands from chash properly
struct hashtable_struct * head;

void run_threads(void);
void* handleCommand(void* arg);



#endif 
