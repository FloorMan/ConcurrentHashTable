#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
// Uncomment out the include paths when compiling with linux and comment out the windows include path.
// Reverse it for windows.
//#include <pthread.h> // Used for creating threads
//#include <unistd.h> // Used for the sleep() function

#include <libunistd/unistd/unistd.h>
#include <libunistd/unistd/pthread.h>


// "Libunistd supports all the common Linux POSIX calls,⚠️⚠️ EXCEPT FORK(). PLEASE DON'T FORK ⚠️⚠️. Use C++ standard threads or libunistd's POSIX pthreads instead.""


typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

// Holds thread number, name, status, etc...
typedef struct thread_struct
{
  int threadNum;
  char threadName[50]

} thread_t; 




// Definition for the HashTable
// Need to add size
hashRecord HashTable[];

// Definition for the Lock
// pthread luckily provides us with a handy Read-Write lock struct
pthread_rwlock_t lock;

int main(){

  // We start off by reading in all of the commands from the 








    return 0;
}





// This function creates a thread and returns it
thread_t * createThread(){
  thread_t * newThread = (thread_t *) malloc(sizeof(thread_t));


    return newThread;
}
