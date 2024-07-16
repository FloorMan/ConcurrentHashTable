# README

## Concurrent Hash Table

This 

### Commands.txt
The ```commands.txt``` file is the lifeblood of this program. **Make sure that the file is placed in the same directory as ```chash.c```**. If the file is placed anywhere else the program will not find it and won't produce any output. \

Each command has to be on it's own line followed by a newline character \

⚠️All insert operations must precede delete operations.⚠️ \
&emsp; &emsp; This ensures that all entries are within the hashtable.


| Command | Parameters                   | Description |
|---------------------|----------|------------------------------------------|
| threads | \<number of threads\>,0         | The number of commands to be processed in the file. This allows you to setup a dynamic array of pthread_t structs. This is always the first line of the command file.  Each command after this runs in a separate thread, so if you have 12 commands, this line should read threads 12,0                                                   |
| insert  |\<name\>,\<salary\>	          | inserts or updates the data for the given name and salary value. |
| delete  | \<name\>,0              | Name whose record is to be deleted from the list. |
| search  | \<name\>,0              | Name whose record is to be retrieved and printed to the output file. |
| print   | 0,0                   | Print the entire contents of the list to the output file. |


### Functions

Used inside of ```chash.c```. Self explanatory.

| Function               |                          Description                                   |
|------------------------|------------------------------------------------------------------------|
| insert(key, values)    |This function adds or updates a key-value pair in the hash table. It computes the hash value of the key, acquires a write lock, updates the value if the key exists, or inserts a new node if it doesn't, then releases the lock.  |
| delete(key) | This function deletes a key-value pair from the hash table if it exists. It computes the hash value, acquires a write lock, removes the node if the key is found, and then releases the lock.                                                |              
| search(key) | This function searches for a key-value pair in the hash table and returns the value if it exists. It computes the hash value, acquires a read lock, searches the linked list, and returns the value if found or NULL if not, then releases the lock.|



## Compile Instructions

### 🪟 WINDOWS
When compiling using Windows, make sure to add the following #include path to your VScode instance so you don't get any wierd bugs. The following library is used to bring all the wonderful magic of unistd to Windows.
<p style="text-align: center;">🔗<a href="https://github.com/robinrowe/libunistd">Robin Rowe's libunistd</a></p>

Clone the repo to the same directory you have ```chash.c``` located. Then you want to update your includePath in VSCode to include libunistd [unistd.h and pthreads.h]



<img src="WindowsInstalDirect.png" alt="drawing" width="500"/> \
 <span style="color:gray">Obviously replace the "Directory you are currently in" with the directory of chash.c. VSCode will have the top two automatically formatted for you when you start a new project, all you have to do is add a new path with ** appended to the end to include the libunistd folder. </span>

We want to make sure we comment out the originals:
```c
#include <pthread.h>
#include <unistd.h>
```
and include these two paths:
```c
#include <libunistd/unistd/pthread.h>
#include <libunistd/unistd/unistd.h>
```

<img src="WindowsChangeLines.png" alt="drawing" width="500"/>

Finally, we include the following flag when compiling: 
```bash
~$ gcc  chash.c  -lpthread
```



### 🐧 LINUX
All we need to do for compiling with Linux is to include the following flag since unistd is a UNIX library:
```bash
~$ gcc  chash.c  -lpthread
```


## Important Notes
"Libunistd supports all the common Linux POSIX calls,⚠️⚠️ EXCEPT FORK(). PLEASE DON'T FORK ⚠️⚠️. Use C++ standard threads or libunistd's POSIX pthreads instead.""

## TODO
* Implement OS detection
    * Two builds, one for 🪟 Windows and one for 🐧 Linux. (Two different versions of chash.c we use, only difference is the libraries used and compile instr.)  
* Implement Hashing Alg.
* Implement HashTable
* Ktinker GUI
*
*


