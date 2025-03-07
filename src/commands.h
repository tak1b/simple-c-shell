/*
 * File: commands.h
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This header file declares the prototypes for built-in command functions and utility functions.
 *   These functions are implemented in commands.c and are used by the main shell in myshell.c.
 *
 * Note:
 *   This file allows the main shell to call command functions without needing their full definitions.
 */

 #ifndef COMMANDS_H
 #define COMMANDS_H
 
 #include "myshell.h"  // Include common definitions
 
 /* 
  * Built-in command functions declarations.
  * Each function takes an array of strings (the tokenized command line) as its argument.
  */
 void kShellHelp(char **args);   // Display the help message
 void kShellClear(char **args);  // Clear the screen using system("clear")
 void kShellDir(char **args);    // List directory contents
 void kShellEcho(char **args);   // Print text to the terminal
 void kShellCD(char **args);     // Change directory or display current directory
 
 /*
  * Utility functions declarations.
  * These help with processing background tokens and executing commands.
  */
 int runBackground(char **args);  // Checks for "&" and returns a flag
 void executeExecute(char **args, int bg);  // Executes the given command (built-in or external)
 
 #endif // COMMANDS_H
 