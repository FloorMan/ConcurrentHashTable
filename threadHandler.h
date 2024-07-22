#ifndef THREADHANDLER_H
#define THREADHANDLER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sys/time.h"
#include "parseCommands.h"
#include "chash.h"

  //Create an arrays of threads (needed to be fixed accordingly reading input from files)

extern pthread_t *threads;
// Needs head of hashtable to call commands from chash properly
extern struct hashtable_struct * head;

int run_threads(int numThreads, struct hashtable_struct * head, struct command_t ** cmds);
void* handleCommand(void* arg);
long long current_timestamp();



#endif 
