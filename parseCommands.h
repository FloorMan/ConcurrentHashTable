// Header file for parseCommands.c
#ifndef PARSECOMMANDS_H
#define PARSECOMMANDS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
// Uncomment out the include paths when compiling with linux and comment out the windows include path.
// Reverse it for windows.




#define COMMAND_SIZE 10
#define NAME_SIZE 50


//command struct for storing input from commands.txt 
typedef struct command_t{
    char command[COMMAND_SIZE];
    char name[NAME_SIZE]; 
    uint32_t salary;
} Command;


//method prototypes
Command ** processInputs(FILE *ptr);
void printCommand(Command * command);
void printCommands(Command ** commandsArray);


#endif