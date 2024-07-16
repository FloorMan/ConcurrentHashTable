/*
                            ⚠️⚠️   IMPORTANT   ⚠️⚠️
When using pthread.h, we need to compile the program with the flag -lpthread
                    ╭―――――――------------------―――――――╮
                    │ ~$ gcc  chash.c  -lpthread     │
                    ╰―――――――------------------―――――――╯
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Uncomment out the include paths when compiling with linux and comment out the windows include path.
// Reverse it for windows.
#include <pthread.h> // Used for creating threads
#include <unistd.h> // Used for the sleep() function

//#include <libunistd/unistd/unistd.h>
//#include <libunistd/unistd/pthread.h>


// "Libunistd supports all the common Linux POSIX calls,⚠️⚠️ EXCEPT FORK(). PLEASE DON'T FORK ⚠️⚠️. Use C++ standard threads or libunistd's POSIX pthreads instead.""


// Defines the size of our hashTable
#define TABLE_SIZE 1000

// Defines the structure for each hash record
typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} HashRecord;


// Defines the structure for the entire hash table.
typedef struct hashtable_struct
{
  int count;
  int size;
  HashRecord ** records;
} HashTable;


// Holds thread number, name, status, etc...
typedef struct thread_struct
{
  int threadNum;
  char threadName[50];
} thread_t; 


// Definition for the Lock
// pthread luckily provides us with a handy Read-Write lock struct
pthread_rwlock_t lock;

int main(){

  // We start off by reading in all of the commands from the commands.txt file


    return 0;
}


// =================================================================================
//  ------------------------- HASHTABLE HELPER FUNCTIONS --------------------------
// =================================================================================

// Returns a new, empty HashTable
HashTable * initHashTable(){
  HashTable * newHashTable = (HashTable*)malloc(sizeof(HashTable));
  newHashTable->count = 0;
  newHashTable->size = TABLE_SIZE;

  // Allocating space for the main array of HashRecords, holds TABLE_SIZE amt of HashRecords
  HashRecord ** newRecordArray = (HashRecord **)malloc(sizeof(HashRecord *) * TABLE_SIZE);
  newHashTable->records = newRecordArray;

  return newHashTable;
}

// 0 = added to table | 1 = alr in table, updated
// Adds a name to the table, acquires a write lock, updates node, releases lock.
int insert(char * name, uint32_t salary){


  return 0;
}

void delete(char * name){

}

void search(char * name){

}


// =================================================================================
//  ------------------------- THREAD HELPER FUNCTIONS --------------------------
// =================================================================================

// This function creates a thread and returns it
thread_t * createThread(){
  thread_t * newThread = (thread_t *) malloc(sizeof(thread_t));


    return newThread;
}
