#include "threadHandler.h"

pthread_cond_t readerwait = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rlock = PTHREAD_MUTEX_INITIALIZER;

bool writer = false;
int readers = 0;

void run_threads(void){
  
  //Create threads
  for (int i = 0; i < numThreads; i++){
     if (pthread_create(&threads[i], NULL, handleCommand, (void*)&cmds[i]) != 0) {
          perror("Error creating thread %d\n", i);
          exit(__LINE__);
        }
  }

  //Wait for all threads to finish
  for (int i = 0; i < numThreads; i++){
    pthread_join(threads[i], NULL);
  }

}
void* handleCommand(void* arg) {
    struct command_t *cmd = (struct command_t *)arg;
    
    if (strcmp(cmd->command, "insert") == 0) {
      //writer lock
      pthread_mutex_lock(&rlock);
      while(writer)
        pthread_cond_wait(&readerwait, &rlock);
      
      writer = true;

      while(readers != 0){
        pthread_cond_wait(&readerwait, &rlock);
      }
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: WRITE LOCK ACQUIRED", current_timestamp());

      // call insert function
      insert(head, cmd->name, cmd->salary);

      //writer unlock
      pthread_mutex_unlock(&lock);
      writer = false;
      pthread_cond_broadcast(&readerwait);
      printf("%llu: WRITE LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "delete") == 0) {
      //writer lock
      pthread_mutex_lock(&rlock);
      while(writer)
        pthread_cond_wait(&readerwait, &rlock);
      
      writer = true;

      while(readers != 0){
        pthread_cond_wait(&readerwait, &rlock);
      }
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: WRITE LOCK ACQUIRED", current_timestamp());

      // call delete function
      delete(head, cmd->name);

      //writer unlock
      pthread_mutex_unlock(&lock);
      writer = false;
      pthread_cond_broadcast(&readerwait);
      printf("%llu: WRITE LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "search") == 0) {
      // reader lock
      pthread_mutex_lock(&rlock);
      while(writer){
        pthread_cond_wait(&readerwait,&rlock);
      }
      readers++;
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: READ LOCK ACQUIRED", current_timestamp());

      // call search function
      search(head, cmd->name);

      //reader unlock
      pthread_mutex_unlock(&lock);  
      pthread_mutex_lock(&rlock);
      readers--;
      if (readers == 0){
        pthread_cond_broadcast(&readerwait);
      }
      pthread_mutex_unlock(&rlock);
      printf("%llu: READ LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "print") == 0) {
      // reader lock
      pthread_mutex_lock(&rlock);
      while(writer){
        pthread_cond_wait(&readerwait,&rlock);
      }
      readers++;
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: READ LOCK ACQUIRED", current_timestamp());

      // call print function
      printTable(head);

      //reader unlock
      pthread_mutex_unlock(&lock);  
      pthread_mutex_lock(&rlock);
      readers--;
      if (readers == 0){
        pthread_cond_broadcast(&readerwait);
      }
      pthread_mutex_unlock(&rlock);
      printf("%llu: READ LOCK RELEASED", current_timestamp());

    } else {
        printf("Unknown command %s\n", cmd->command);
    }

    return NULL;
}

// =================================================================================
//  ----------------------------- TIMESTAMP FUNCTION ------------------------------
// =================================================================================
// the timestamp function provided by Proffesor Aedo to get 
// the current time in millisenconds since January 1, 1970 

long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL); // get current time
  long long microseconds = (te.tv_sec * 1000000) + te.tv_usec; // calculate milliseconds
  return microseconds;
}

