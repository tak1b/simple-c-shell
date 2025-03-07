/*
 * File: utility.h
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This header file contains the implementations of the built-in shell commands and utility functions.
 *   In this version, we do not explicitly fork a child process. Instead, built-in commands are executed
 *   directly in the main process, and external commands are executed via a system() call.
 */

 #ifndef UTILITY_H
 #define UTILITY_H
 
 #include "myshell.h"  // Include shared definitions
 
 // Structure that maps built-in command names to their corresponding functions
 struct command {
     char* name;
     void (*func)(char **args);
 };
 
 // Array of built-in commands with their function pointers
 struct command commands[] = {
     {"clr", kShellClear},   // Clears the terminal
     {"dir", kShellDir},     // Lists directory contents
     {"echo", kShellEcho},   // Prints text to the terminal
     {"help", kShellHelp}    // Displays the help manual
 };
 
 /**
  * cmdLength:
  *   Returns the number of built-in commands available.
  *
  * Returns:
  *   The count of built-in commands.
  */
 int cmdLength() {
     return sizeof(commands) / sizeof(struct command);
 }
 
 /**
  * runBackground:
  *   Scans the command arguments for the background execution symbol "&".
  *   If found, it removes the token from the argument list.
  *
  * args: Array of command arguments.
  *
  * Returns:
  *   1 if "&" is found (indicating background execution), 0 otherwise.
  *
  * Note:
  *   Background execution is not supported in this single-process mode.
  */
 int runBackground(char **args) {
     int i = 0;
     while (args[i] != NULL) {
         if (strcmp(args[i], "&") == 0) {
             args[i] = NULL;  // Remove the background token
             return 1;
         }
         i++;
     }
     return 0;
 }
 
 /**
  * executeExecute:
  *   Executes the given command without explicitly forking a child process.
  *   If the command is built-in (e.g., clr, dir, echo, help), its function is called directly.
  *   Otherwise, the command is assembled into a single string and executed using system().
  *
  * args: Array of command arguments.
  * bg: Background execution flag (ignored in this mode; a warning is printed if set).
  */
 void executeExecute(char **args, int bg) {
     // Warn if background execution is requested since it is not supported.
     if (bg == 1) {
         printf("Warning: Background execution not supported in single-process mode. Running in foreground.\n");
     }
     
     // Check for a built-in command match
     int found = 0;
     for (int i = 0; i < cmdLength(); i++) {
         if (strcmp(args[0], commands[i].name) == 0) {
             commands[i].func(args);  // Execute the built-in command
             found = 1;
             break;
         }
     }
     
     if (!found) {
         // Assemble the external command as a single string
         char commandLine[1024] = "";
         for (int i = 0; args[i] != NULL; i++) {
             strcat(commandLine, args[i]);
             if (args[i + 1] != NULL) {
                 strcat(commandLine, " ");
             }
         }
         // Execute the external command using system()
         int ret = system(commandLine);
         if (ret == -1) {
             perror("System call failed");
         }
     }
 }
 
 /**
  * kShellHelp:
  *   Displays the help manual using the system's "more" command.
  *   Changes directory into the "manual" folder to locate the "readme.md" file,
  *   then displays it. Does not exit the shell.
  *
  * args: Array of command arguments (unused).
  */
 void kShellHelp(char **args) {
    printf("\nMy Shell Help Manual\n");
    printf("--------------------\n");
    printf("cd [directory]   : Change the current directory. If no directory is provided, displays the current working directory.\n");
    printf("clr              : Clear the terminal screen.\n");
    printf("dir [directory]  : List the contents of a directory. If no directory is specified, lists the current directory.\n");
    printf("environ          : Display all environment variables.\n");
    printf("echo [text]      : Display the provided text on the screen.\n");
    printf("help             : Display this help information.\n");
    printf("pause            : Pause the shell until Enter is pressed.\n");
    printf("quit             : Exit the shell.\n\n");
}

 
 /**
  * kShellClear:
  *   Clears the terminal screen by calling the system's "clear" command.
  *
  * args: Array of command arguments (unused).
  */
 void kShellClear(char **args) {
     system("clear");
     printf("Cleared shell\n");
 }
 
 /**
  * kShellEcho:
  *   Prints the provided text arguments to the terminal.
  *
  * args: Array of command arguments; args[0] is "echo" and subsequent elements are text.
  */
 void kShellEcho(char **args) {
     printf("%s", CYN);  // Set output color to cyan
     char **arg = args + 1;  // Skip the command name
     while (*arg) {
         printf("%s ", *arg);
         arg++;
     }
     printf("%s\n", WHT);  // Reset output color and add newline
 }
 
 /**
  * kShellCD:
  *   Changes the current working directory. If no directory is specified,
  *   it changes to the home directory and then lists the directory contents.
  *
  * args: Array of command arguments.
  */
 void kShellCD(char **args) {
     if (args[1] == NULL) {
         chdir(getenv("HOME"));
     } else {
         chdir(args[1]);
     }
     system("ls");
     printf("%s", WHT);
 }
 
 /**
  * kShellDir:
  *   Lists the contents of a directory. If an argument is provided,
  *   that directory is listed; otherwise, the current directory is used.
  *
  * args: Array of command arguments.
  */
 void kShellDir(char **args) {
     char input[100] = "ls -al ";  // Base command for detailed listing
     if (args[1]) {
         strcat(input, args[1]);  // Append the specified directory if provided
     }
     system(input);  // Execute the directory listing command
 }
 
 #endif  // End of UTILITY_H
 