#include "threadHandler.h"`

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

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
    pthread_rwlock_t *p = (pthread_rwlock_t *)arg;  
    struct command_t *cmd = (struct command_t *)arg;

    if (strcmp(cmd->command, "insert") == 0) {
      //try writer lock. if fails exit 
      if (pthread_rwlock_wrlock(p) != 0) {
        perror("reader_thread: pthread_rwlock_wrlock error");
        exit(__LINE__);
      }
      // call insert function
      insert(head, cmd->name, cmd->salary);
      //try unlock. if fails exit
      if (pthread_rwlock_unlock(p) != 0) {
        perror("reader thread: pthred_rwlock_unlock error");
        exit(__LINE__);
      }   
    } else if (strcmp(cmd->command, "delete") == 0) {
      //try writer lock. if fails exit 
      if (pthread_rwlock_wrlock(p) != 0) {
        perror("reader_thread: pthread_rwlock_wrlock error");
        exit(__LINE__);
      }
      // call delete function
      delete(head, cmd->name);
      //try unlock. if fails exit
      if (pthread_rwlock_unlock(p) != 0) {
        perror("reader thread: pthred_rwlock_unlock error");
        exit(__LINE__);
      }   

    } else if (strcmp(cmd->command, "search") == 0) {
      //try reader lock. if fails exit 
      if (pthread_rwlock_rdlock(p) != 0) {
        perror("reader_thread: pthread_rwlock_rdlock error");
        exit(__LINE__);
      }
      // call search function
      search(head, cmd->name);
      //try unlock. if fails exit
      if (pthread_rwlock_unlock(p) != 0) {
        perror("reader thread: pthred_rwlock_unlock error");
        exit(__LINE__);
      }   
    } else if (strcmp(cmd->command, "print") == 0) {
       if (pthread_rwlock_rdlock(p) != 0) {
        perror("reader_thread: pthread_rwlock_rdlock error");
        exit(__LINE__);
      }
      // call print function
      printTable(head);
      //try unlock. if fails exit
      if (pthread_rwlock_unlock(p) != 0) {
        perror("reader thread: pthred_rwlock_unlock error");
        exit(__LINE__);
      }   
    } else {
        printf("Unknown command %s\n", cmd->command);
    }

    return NULL;
}


