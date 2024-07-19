#include "threadHandler.h"`

  // Initialize commands (needed to be fixed accordingly reading input from files)
  for (int i = 0; i < numThreads; i++){
    strcpy(cmds[i].command, "insert");
    cmds[i].name = "John";
    cmds[i].salary = 1000;
  }

  //Create threads
  for (int i = 0; i < numThreads; i++){
     if (pthread_create(&threads[i], NULL, handleCommand, (void*)&cmds[i]) != 0) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
  }

  //Wait for all threads to finish
  for (int i = 0; i < numThreads; i++){
    pthread_join(threads[i], NULL);
  }


void* handleCommand(void* arg) {
    struct command_t *cmd = (struct command_t *)arg;

    if (strcmp(cmd->command, "insert") == 0) {
      // call insert function
       
    } else if (strcmp(cmd->command, "delete") == 0) {
      // call delete function
       

    } else if (strcmp(cmd->command, "search") == 0) {
      // call search function
       
    } else if (strcmp(cmd->command, "print") == 0) {
      // call print function
      
    } else {
        printf("Unknown command %s\n", cmd->command);
    }

    return NULL;
}


