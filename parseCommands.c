#include "parseCommands.h"

/*
// =================================================================================
//  ------------------------- processInputs --------------------------
// =================================================================================
                Reads the inputs in the command.txt file, parses 
                the data into an array of struct command_t, and
                    returns a pointer to the array. 
*/


Command ** processInputs(FILE *ptr)
{
    //data declaration
    char str0[COMMAND_SIZE] = {0};
    char str1[NAME_SIZE] = {0};
    int num;
    int numThreads;
    Command ** commandsArray; 

    //checking if file is opened successfully
    if (NULL == ptr) printf("file can't be opened \n");

    // reading first line of file
    fscanf(ptr, "%[^,;],%d,%d", str0, &numThreads, &num);

    // validating first line of file
    if(strcmp(str0, "threads") == 0){
        // Creating array of structs
        commandsArray = (Command **) malloc(sizeof(Command *) * (numThreads + 1));

        // Stores the thread command in the first index of the array
        commandsArray[0] = (Command *) malloc(sizeof(Command));
        strcpy(commandsArray[0]->command, str0);
        strcpy(commandsArray[0]->name, "threads");
        commandsArray[0]->salary = numThreads;
        
        for(int i = 1; i < numThreads + 1; i++) 
        {
            // Creating new line present at the beginning of each subsequent line
            char c = fgetc(ptr);
            while(c != '\n' && c != EOF) c = fgetc(ptr);
            
            fscanf(ptr, "%[^,;],%[^,;],%d", str0, str1, &num);

            // Allocating memory for each struct
            commandsArray[i] = (Command *) malloc(sizeof(Command));
            strcpy(commandsArray[i]->command, str0);
            strcpy(commandsArray[i]->name, str1);
            commandsArray[i]->salary = num;
        }
    }
    else printf("First line invalid. Please check your file.");

    return commandsArray;
}


//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             GETTER FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Returns the amount of threads to be created
int getNumThreads(Command ** commandsArray)
{
    return commandsArray[0]->salary;
}


//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             PRINT FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void printCommand(Command * command)
{
  printf("|%s| |%s| |%d|\n", command->command,command->name,command->salary);
}

// Printing complete command_t struct information
// Only run AFTER processInputs or else numThreads will be 0
void printCommands(Command ** commandsArray)
{
    int numThreads = commandsArray[0]->salary;
  
    printf("-----------------------------------------\n");
    for (int i = 0; i < numThreads + 1; i++)
    {
        printCommand(commandsArray[i]);
    }
}


//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             FREE FUNCTIONS
//              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Frees the Command array passed
void freeCommands(Command ** cmds) {

    int numThreads = cmds[0]->salary;

    // Go through and free all of the memory allocated for the command_t structs
    for (int i = 0; i < numThreads + 1; i++) {
        free(cmds[i]);
    }
    free(cmds);
}