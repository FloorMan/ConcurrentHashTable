/*
                            ⚠️⚠️   IMPORTANT   ⚠️⚠️
When using pthread.h, we need to compile the program with the flag -lpthread
                    ╭―――――――------------------―――――――╮
                    │ ~$ gcc  chash.c  -lpthread     │
                    ╰―――――――------------------―――――――╯
*/
// Header File
#include "chash.h"

struct command_t {
    char command[8];       // 1st input per line: insert, delete, search, or print
    char* name;        // 2nd input per line: may be an integer or a string name
    int salary;           // 3rd input per line: integer
};




int main(){

  // We start off by reading in all of the commands from the commands.txt file

  // Testing out the table Init, hash function, insert, search, delete



  //Create an arrays of threads (needed to be fixed accordingly reading input from files)
  int numThreads = 11;
  pthread_t threads[numThreads];
  struct command_t cmds[numThreads];

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






    return 0;
}

// =================================================================================
//  ------------------------- HASHTABLE HELPER FUNCTIONS --------------------------
// =================================================================================

//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             HASH FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Returns an unsigned 32 bit integer (hash)
// Uses the Jenkin's "One at a time" hash function
uint32_t calculateHash(const uint8_t * name, size_t length){
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += name[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}




//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             INIT FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Returns a pointer to a new, empty HashTable
HashTable * initHashTable(){
  HashTable * newHashTable = (HashTable*)malloc(sizeof(HashTable));
  newHashTable->count = 0;
  newHashTable->size = 0;
  newHashTable->head = NULL;


  return newHashTable;
}

// Returns a pointer to a new Hash Record
HashRecord * initHashRecord(char * name, uint32_t salary){
  HashRecord * newRecord = (HashRecord *)malloc(sizeof(HashRecord));

  strcpy(newRecord->name, name);
  newRecord->salary = salary;
  newRecord->next = NULL;
  newRecord->hash = calculateHash(name, strlen(name));

  return newRecord;
}


//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             FREE FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO 
// Implement linked list deletion for records

// 😠😠😠 DESTROY BUILD DESTROY 😠😠😠
// Returns 0 = record doesn't exist (NULL) | 1 = Frees name and record
int destroyRecord(HashRecord * record){
  if (record == NULL)
    return 0;
  free(record);

  return 1;
}



// 😠😠😠 DESTROY BUILD DESTROY 😠😠😠
// Goes thru the records array and frees each record
void destroyTable(HashTable * table){

  if (table == NULL)
    return;

  HashRecord * head = table->head; 
  HashRecord * next;

  while (head != NULL){
    next = head->next;
    destroyRecord(head);
    head = next;
  }

  free(table);
}


//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             TABLE FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Returns 0 = added to table | 1 = alr in table, updated
// Adds a name to the table, acquires a write lock, updates node, releases lock.
int insert(char * name, uint32_t salary){


  return 0;
}


// Returns 0 = not in table | 1 = found and deleted
// Finds the name in the table and deletes it
int delete(char * name){


  return 0;
}

// Returns name if found, NULL if not in table
// Finds a name in the table
char * search(char * name){
  char * foundName = NULL;

  return foundName;
}


// =================================================================================
//  ------------------------- THREAD HELPER FUNCTIONS --------------------------
// =================================================================================

// This function creates a thread and returns it
thread_t * createThread(){
  thread_t * newThread = (thread_t *) malloc(sizeof(thread_t));


    return newThread;
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
