#include "threadHandler.h"

pthread_cond_t readerwait = PTHREAD_COND_INITIALIZER;
pthread_cond_t insertwait = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rlock = PTHREAD_MUTEX_INITIALIZER;

int writer = 0;
int readers = 0;
int inserts = 0;

pthread_t *threads;
struct hashtable_struct * head;

int run_threads(int numThreads, struct hashtable_struct * HashTable, struct command_t ** cmds){
  head = HashTable;
  threads =(pthread_t *) malloc (numThreads*sizeof(pthread_t));
  //Create threads
  for (int i = 0; i < numThreads; i++){
     if (pthread_create(&threads[i], NULL, handleCommand, (void*)&cmds[i]) != 0) {
          fprintf(stderr,"Error creating thread %d\n",i);
          perror("");
          exit(__LINE__);
        }
  }

  //Wait for all threads to finish
  for (int i = 0; i < numThreads; i++){
    pthread_join(threads[i], NULL);
  }
  free(threads);
  return 1;
}
void* handleCommand(void* arg) {
    struct command_t *cmd = (struct command_t *)arg;
    
    if (strcmp(cmd->command, "insert") == 0) {
      //writer lock
      pthread_mutex_lock(&rlock);

      inserts++;

      while(writer)
        printf("%llu: Awaiting other writers", current_timestamp());
        pthread_cond_wait(&readerwait, &rlock);
        printf("%llu: Insert Awaken", current_timestamp());
      
      writer = 1;

      while(readers != 0){
        printf("%llu: Awaiting readers", current_timestamp());
        pthread_cond_wait(&readerwait, &rlock);
        printf("%llu: Insert Awaken", current_timestamp());
      }
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: WRITE LOCK ACQUIRED", current_timestamp());

      // call insert function
      insert(head, cmd->name, cmd->salary);
      printf("%llu: INSERT, %s, %llu", current_timestamp(),cmd->name,cmd->salary);

      //writer unlock
      pthread_mutex_unlock(&lock);
      pthread_mutex_lock(&rlock);
      inserts--;
      if (inserts == 0){
        pthread_cond_broadcast(&insertwait);
        printf("%llu: Signal deletes", current_timestamp());
      }
      pthread_mutex_unlock(&rlock);
      writer = 0;
      pthread_cond_broadcast(&readerwait);
      printf("%llu: WRITE LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "delete") == 0) {
      //writer lock
      pthread_mutex_lock(&rlock);
      while(writer)
        printf("%llu: Awaiting other writers", current_timestamp());
        pthread_cond_wait(&readerwait, &rlock);
        printf("%llu: Delete Awaken", current_timestamp());
      
      writer = 1;

      while(readers != 0){
        printf("%llu: Awaiting readers", current_timestamp());
        pthread_cond_wait(&readerwait, &rlock);
        printf("%llu: Delete Awaken", current_timestamp());
      }
      while(inserts != 0){
        printf("%llu: Awaiting insers", current_timestamp());
        pthread_cond_wait(&insertwait, &rlock);
        printf("%llu: Delete Awaken", current_timestamp());
      }
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: WRITE LOCK ACQUIRED", current_timestamp());

      // call delete function
      delete(head, cmd->name);
      printf("%llu: DELETE, %s", current_timestamp(),cmd->name);

      //writer unlock
      pthread_mutex_unlock(&lock);
      writer = 0;
      pthread_cond_broadcast(&readerwait);
      printf("%llu: Signaled readers and writers", current_timestamp());
      printf("%llu: WRITE LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "search") == 0) {
      // reader lock
      pthread_mutex_lock(&rlock);
      while(writer){
        printf("%llu: Awaiting writers", current_timestamp());
        pthread_cond_wait(&readerwait,&rlock);
        printf("%llu: Search Awakened", current_timestamp());
      }
      readers++;
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: READ LOCK ACQUIRED", current_timestamp());

      // call search function
      search(head, cmd->name);
      printf("%llu: SEARCH, %s", current_timestamp(),cmd->name);

      //reader unlock
      pthread_mutex_unlock(&lock);  
      pthread_mutex_lock(&rlock);
      readers--;
      if (readers == 0){
        pthread_cond_broadcast(&readerwait);
        printf("%llu: Signal all awaiting readers", current_timestamp());
      }
      pthread_mutex_unlock(&rlock);
      printf("%llu: READ LOCK RELEASED", current_timestamp());

    } else if (strcmp(cmd->command, "print") == 0) {
      // reader lock
      pthread_mutex_lock(&rlock);
      while(writer){
        printf("%llu: Awaiting writers", current_timestamp());
        pthread_cond_wait(&readerwait,&rlock);
        printf("%llu: Search Awakened", current_timestamp());
      }
      readers++;
      pthread_mutex_unlock(&rlock);
      pthread_mutex_lock(&lock);
      printf("%llu: READ LOCK ACQUIRED", current_timestamp());

      // call print function
      printTable(head);
      printf("%llu: print table", current_timestamp());

      //reader unlock
      pthread_mutex_unlock(&lock);  
      pthread_mutex_lock(&rlock);
      readers--;
      if (readers == 0){
        pthread_cond_broadcast(&readerwait);
        printf("%llu: Signal all awaiting readers", current_timestamp());
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
