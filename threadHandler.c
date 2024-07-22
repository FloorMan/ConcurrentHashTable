#include "threadHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

//for outputing to file
FILE *outptr;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t *threads;
struct hashtable_struct * head;

int locks_acquired = 0;
int locks_released = 0;
int active_writers = 0;
int waiting_writers = 0;
int active_readers = 0;

int run_threads(int numThreads, struct hashtable_struct * HashTable, struct command_t ** cmds){
  //creating/opening output.txt file to write to.
  outptr = fopen("output.txt", "w");
  
  head = HashTable;
  threads = (pthread_t *) malloc(numThreads * sizeof(pthread_t));
  //printf("Running %d threads\n", numThreads);
  fprintf(outptr,"Running %d threads\n", numThreads);
  for (int i = 0; i < numThreads; i++){
     if (pthread_create(&threads[i], NULL, handleCommand, (void*)cmds[i+1]) != 0) {
          fprintf(stderr, "Error creating thread %d\n", i);
          perror("");
          exit(__LINE__);
        }
  }

  for (int i = 0; i < numThreads; i++){
    pthread_join(threads[i], NULL);
  }
  free(threads);
  
  //printf("Finished all threads.\n");
  fprintf(outptr, "Finished all threads.\n");
  //printf("Number of lock acquisitions:  %d\n", locks_acquired);
  fprintf(outptr,"Number of lock acquisitions:  %d\n", locks_acquired);
  //printf("Number of lock releases:  %d\n", locks_released);
  fprintf(outptr,"Number of lock releases:  %d\n", locks_released);
  fclose(outptr);
  return 1;
}

void* handleCommand(void* arg) {
    struct command_t *cmd = (struct command_t *)arg;

    if (strcmp(cmd->command, "insert") == 0) {
        pthread_mutex_lock(&lock);
        locks_acquired++;
        waiting_writers++;
        while (active_readers > 0 || active_writers > 0) {
            //printf("%llu: WAITING ON INSERTS\n", current_timestamp());
            fprintf(outptr,"%llu: WAITING ON INSERTS\n", current_timestamp());
            pthread_cond_wait(&cond, &lock);
        }
        waiting_writers--;
        active_writers++;
        pthread_mutex_unlock(&lock);

        //printf("%llu: WRITE LOCK ACQUIRED\n", current_timestamp());
        fprintf(outptr, "%llu: WRITE LOCK ACQUIRED\n", current_timestamp());
        insert(head, cmd->name, cmd->salary);
        //printf("%llu: INSERT,%u,%s,%llu\n", current_timestamp(), calculateHash(cmd->name, strlen(cmd->name)), cmd->name, cmd->salary);
        fprintf(outptr, "%llu: INSERT,%u,%s,%llu\n", current_timestamp(), calculateHash(cmd->name, strlen(cmd->name)), cmd->name, cmd->salary);

        pthread_mutex_lock(&lock);
        active_writers--;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);

        //printf("%llu: WRITE LOCK RELEASED\n", current_timestamp());
        fprintf(outptr, "%llu: WRITE LOCK RELEASED\n", current_timestamp());
        locks_released++;

    } else if (strcmp(cmd->command, "delete") == 0) {
        pthread_mutex_lock(&lock);
        locks_acquired++;
        waiting_writers++;
        while (active_readers > 0 || active_writers > 0) {
            pthread_cond_wait(&cond, &lock);
        }
        waiting_writers--;
        active_writers++;
        pthread_mutex_unlock(&lock);

        //printf("%llu: DELETE AWAKENED\n", current_timestamp());
        fprintf(outptr, "%llu: DELETE AWAKENED\n", current_timestamp());
        //printf("%llu: WRITE LOCK ACQUIRED\n", current_timestamp());
        fprintf(outptr, "%llu: WRITE LOCK ACQUIRED\n", current_timestamp());
        delete(head, cmd->name);
        //printf("%llu: DELETE,%s\n", current_timestamp(), cmd->name);
        fprintf(outptr, "%llu: DELETE,%s\n", current_timestamp(), cmd->name);

        pthread_mutex_lock(&lock);
        active_writers--;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);

        //printf("%llu: WRITE LOCK RELEASED\n", current_timestamp());
        fprintf(outptr, "%llu: WRITE LOCK RELEASED\n", current_timestamp());
        locks_released++;

    } else if (strcmp(cmd->command, "search") == 0) {
        pthread_mutex_lock(&lock);
        locks_acquired++;
        while (active_writers > 0 || waiting_writers > 0) {
            pthread_cond_wait(&cond, &lock);
        }
        active_readers++;
        pthread_mutex_unlock(&lock);

        //printf("%llu: READ LOCK ACQUIRED\n", current_timestamp());
        fprintf(outptr, "%llu: READ LOCK ACQUIRED\n", current_timestamp());
        HashRecord * result = search(head, cmd->name);
        if (result == NULL) {
            //printf("%llu: SEARCH: NOT FOUND NOT FOUND\n", current_timestamp());
            fprintf(outptr, "%llu: SEARCH: NOT FOUND NOT FOUND\n", current_timestamp());
        } else {
            //printf("%llu: SEARCH: %u,%s,%llu\n", current_timestamp(), result->hash, result->name, result->salary);
            fprintf(outptr, "%llu: SEARCH: %u,%s,%llu\n", current_timestamp(), result->hash, result->name, result->salary);
        }

        pthread_mutex_lock(&lock);
        active_readers--;
        if (active_readers == 0) {
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&lock);

        //printf("%llu: READ LOCK RELEASED\n", current_timestamp());
        fprintf(outptr, "%llu: READ LOCK RELEASED\n", current_timestamp());
        locks_released++;

    } else if (strcmp(cmd->command, "print") == 0) {
        pthread_mutex_lock(&lock);
        locks_acquired++;
        while (active_writers > 0 || waiting_writers > 0) {
            pthread_cond_wait(&cond, &lock);
        }
        active_readers++;
        pthread_mutex_unlock(&lock);

        //printf("%llu: READ LOCK ACQUIRED\n", current_timestamp());
        fprintf(outptr, "%llu: READ LOCK ACQUIRED\n", current_timestamp());
        printTable(head);

        pthread_mutex_lock(&lock);
        active_readers--;
        if (active_readers == 0) {
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&lock);

        //printf("%llu: READ LOCK RELEASED\n", current_timestamp());
        fprintf(outptr, "%llu: READ LOCK RELEASED\n", current_timestamp());
        locks_released++;

    } else {
          //printf("Unknown command %s\n", cmd->command);
          fprintf(outptr, "Unknown command %s\n", cmd->command);
    }

    return NULL;
}

// =================================================================================
//  ----------------------------- TIMESTAMP FUNCTION ------------------------------
// =================================================================================
// the timestamp function provided by Professor Aedo to get 
// the current time in usenconds since January 1, 1970 

long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL); // get current time
  long long microseconds = (te.tv_sec * 1000000) + te.tv_usec; // calculate useconds
  return microseconds;
}
