/*
//                            ⚠️⚠️   IMPORTANT   ⚠️⚠️
//When using pthread.h, we need to compile the program with the flag -lpthread
//                    ╭―――――――------------------―――――――╮
//                    │ ~$ gcc  chash.c  -lpthread     │
//                    ╰―――――――------------------―――――――╯
*/
// Header File
#include "chash.h"


int main(){

  // We start off by reading in all of the commands from the commands.txt file
  // Utilizing the processInputs function from parseCommands.c
  FILE * fileptr = fopen("commands.txt", "r");
  HashTable * hashTable = readCommands(fileptr);
  fclose(fileptr);

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
  newHashTable->tail = NULL;

  return newHashTable;
}

// Returns a pointer to a new Hash Record
HashRecord * initHashRecord(char * name, uint32_t salary){
  HashRecord * newRecord = (HashRecord *)malloc(sizeof(HashRecord));

  strcpy(newRecord->name, name);
  newRecord->salary = salary;
  newRecord->prev = NULL;
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

// Returns hashRecord if found, NULL if not in table
// Finds a name in the table
HashRecord * search(HashTable * hashTable, char * name){
  HashRecord * foundName = NULL;
  HashRecord * head = hashTable->head;

  uint32_t hash = calculateHash(name, strlen(name));

  while (head != NULL){
    if (head->hash == hash){
      foundName = head;
      return foundName;
    }
    head = head->next;
  }

  return NULL;
}



// Returns 0 = added to table | 1 = alr in table, updated
// Adds a name to the table, acquires a write lock, updates node, releases lock.
int insert(HashTable * hashTable, char * name, uint32_t salary){
  // First we check if the name is already in the table
  // If it is, we update the salary. If not, we add to end of list

  HashRecord * foundName = search(hashTable, name);

  if (foundName != NULL){
    // Update the salary
    foundName->salary = salary;
    // Acquire write lock
    // Release write lock
    return 1;
  }

  HashRecord * newRecord = initHashRecord(name, salary);
  // If the hashTable is empty, add the record to the head
  // If not, add to the end of the list
  if (hashTable->head == NULL){
    hashTable->head = newRecord;
    hashTable->tail = newRecord;
  } else {
    hashTable->tail->next = newRecord;
    newRecord->prev = hashTable->tail;
    hashTable->tail = newRecord;
  }


  return 0;
}


// Returns 0 = not in table | 1 = found and deleted
// Finds the name in the table and deletes it
int delete(HashTable * hashTable, char * name){
  HashRecord * foundName = search(hashTable, name);
  if (foundName == NULL)
    return 0;

  // If the record is the only one in the list, set head/tail to NULL
  if (foundName->prev == NULL && foundName->next == NULL){
    hashTable->head = NULL;
    hashTable->tail = NULL;
  }
  // If the record is the head, set the next record to be the head
  else if (foundName->prev == NULL){
    hashTable->head = foundName->next;
    hashTable->head->prev = NULL;
  }
  // If the record is the tail, set the previous record to be the tail
  else if (foundName->prev == NULL){
    hashTable->tail = foundName->prev;
    hashTable->tail->next = NULL;
  }
  else {
  // If the record is in the middle, set the previous record's next to the next record
    foundName->prev->next = foundName->next;
  }

  destroyRecord(foundName);
  return 0;
}



//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             PRINT FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Prints the entire hash table
void printTable(HashTable * hashTable){
  HashRecord * head = hashTable->head;

  printf("-----------------------Hash Table---------------------------------\n");

  while (head != NULL){
    printf("| Name:    %.10s ", head->name);
    head = head->next;
  }
  printf("\n|");
  head = hashTable->head;
  while (head != NULL){
    printf(" Salary: %.10d -> ", head->salary);
    head = head->next;
  }
  printf("\n");
  head = hashTable->head;
  while (head != NULL){
    printf("| Hash:   %.10d ", head->hash);
    head = head->next;
  }
  printf("|\n");
}

// =================================================================================
//  ------------------------- COMMANDS READ FUNCTIONS --------------------------
// =================================================================================

// Will read in the commands from the commands.txt file and execute the appropriate functions
HashTable * readCommands(FILE * ptr){
  // Command ** cmds from parseCommands.h, holds an array of command_t struct pointers 
  Command ** cmds = processInputs(ptr);
  HashTable * hashTable = initHashTable();

  int numThreads = getNumThreads(cmds);
  int success = 0;



  for (int i = 1; i < numThreads + 1; i++){
    if (strcmp(cmds[i]->command, "insert") == 0){
      success = insert(hashTable, cmds[i]->name, cmds[i]->salary);
      //printf("Insertion success: %d\n", success);
    } else if (strcmp(cmds[i]->command, "delete") == 0){
      success = delete(hashTable, cmds[i]->name);
      //printf("Deletion success: %d\n", success);
    } else if (strcmp(cmds[i]->command, "search") == 0){
      HashRecord * foundRecord = search(hashTable, cmds[i]->name);
      if (foundRecord != NULL){
        printf("Found record: %s\n", foundRecord->name);
      } else {
        printf("Record not found\n");
      }
    } else if (strcmp(cmds[i]->command, "print") == 0){
      printf("Table at command %d:\n", i);
      printTable(hashTable);
    }
  }

  // Free the memory allocated for the commands
  freeCommands(cmds);

  printf("\n\nFinal Table:\n");
  printTable(hashTable);

  return hashTable;
}




// =================================================================================
//  ------------------------- THREAD HELPER FUNCTIONS --------------------------
// =================================================================================

// This function creates a thread and returns it
thread_t * createThread(){
  thread_t * newThread = NULL;
  //newThread = (thread_t *) malloc(sizeof(thread_t));


    return newThread;
}
