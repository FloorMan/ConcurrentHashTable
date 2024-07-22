// Header file for chash.c
#ifndef CHASH_H
#define CHASH_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threadHandler.h"
#include "parseCommands.h"
#include <pthread.h> // Used for creating threads
#include <unistd.h>  // Used for the sleep() function
//#include "libunistd/unistd/unistd.h"
//#include "libunistd/unistd/pthread.h"
// "Libunistd supports all the common Linux POSIX calls,⚠️⚠️ EXCEPT FORK().
// PLEASE DON'T FORK ⚠️⚠️. Use C++ standard threads or libunistd's POSIX
// pthreads instead.""

// Defines the size of our hashTable
#define TABLE_SIZE 1000

// Function prototypes

// Defines the structure for each hash record
typedef struct hash_struct {
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *prev;
  struct hash_struct *next;
} HashRecord;

// Defines the structure for the entire hash table.
typedef struct hashtable_struct {
  int count;
  int size;
  HashRecord *head;
  HashRecord *tail;
} HashTable;

// Definition for the Lock
// pthread luckily provides us with a handy Read-Write lock struct
// pthread_rwlock_t lock;

uint32_t calculateHash(const uint8_t *name, size_t length);
HashTable *initHashTable();
HashRecord *initHashRecord(char *name, uint32_t salary);
int destroyRecord(HashRecord *record);
void destroyTable(HashTable *table);
int insert(HashTable *hashTable, char *name, uint32_t salary);
int delete(HashTable *hashTable, char *name);
HashRecord *search(HashTable *hashTable, char *name);
void printTable(HashTable *hashTable);
HashTable *readCommands(FILE *fileptr);

#endif
