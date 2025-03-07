/*
 * File: commands.c
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This file contains implementations for the built-in commands and utility functions used by the shell.
 *   It handles commands such as "clr", "clear", "dir", "echo", "help", and "cd". The "clear" command is
 *   implemented using ANSI escape codes to clear the screen and then reprint the prompt (showing the current
 *   working directory) so that the user information remains visible immediately after clearing.
 *
 *   The method for clearing the console using ANSI escape codes is based on an article from GeeksforGeeks:
 *   "Clear the console in C language" (https://www.geeksforgeeks.org/clear-console-c-language/).
 *
 * Note:
 *   This implementation does not fork a child process; external commands are executed using system().
 */

 #include "commands.h"
 #include <unistd.h>  // For getcwd()
 
 // ---------------------------------------------------------------------------
 // Static Built-in Command Array
 // ---------------------------------------------------------------------------
 /*
  * The static array "commands" maps built-in command names to their corresponding functions.
  * This array is used in executeExecute() to determine if a command should be handled as built-in.
  */
 static struct command {
     char* name;                 // Name of the command (e.g., "clr", "dir")
     void (*func)(char **args);  // Function pointer to the command's implementation
 } commands[] = {
     {"clr",  kShellClear},
     {"dir",  kShellDir},
     {"echo", kShellEcho},
     {"help", kShellHelp}
 };
 
 /*
  * cmdLength:
  *   Returns the number of built-in commands in the commands array.
  */
 static int cmdLength() {
     return sizeof(commands) / sizeof(commands[0]);
 }
 
 // ---------------------------------------------------------------------------
 // Additional Function: check_cmd
 // ---------------------------------------------------------------------------
 /**
  * check_cmd:
  *   Checks if the command entered by the user is "clear". If so, it clears the screen using ANSI escape codes
  *   and then reprints the prompt (which includes the current working directory) so that user info remains visible.
  *
  *   The ANSI escape sequence "\e[1;1H\e[2J" moves the cursor to the top-left corner and clears the screen.
  *   This technique is based on the article "Clear the console in C language" from GeeksforGeeks:
  *   https://www.geeksforgeeks.org/clear-console-c-language/
  *
  * Parameters:
  *   args: The tokenized command-line arguments (with args[0] expected to be "clear").
  *
  * Returns:
  *   1 if the "clear" command was detected and handled; 0 otherwise.
  */
 int check_cmd(char **args) {
     if (!strcmp(args[0], "clear")) {
         // Clear the screen using ANSI escape codes.
         // "\e[1;1H" moves the cursor to row 1, column 1.
         // "\e[2J" clears the entire screen.
         printf("\e[1;1H\e[2J");  // Source: https://www.geeksforgeeks.org/clear-console-c-language/
 
         // Retrieve the current working directory so we can reprint the prompt.
         char cwd[512];
         if (getcwd(cwd, sizeof(cwd)) != NULL) {
             // Reprint the prompt with current directory info.
             // Format: "MS <current_directory> $~ "
             printf("MS %s $~ ", cwd);
         } else {
             perror("getcwd failed");
             printf("MS $~ ");
         }
         return 1;  // Command handled
     }
     return 0;  // Not the "clear" command
 }
 
 // ---------------------------------------------------------------------------
 // Utility Functions
 // ---------------------------------------------------------------------------
 /**
  * runBackground:
  *   Scans the tokenized command arguments for the "&" symbol, indicating that the command should be run
  *   in the background. Since this shell implementation does not support background execution, "&" is simply
  *   removed.
  *
  * Parameters:
  *   args: The tokenized command-line arguments.
  *
  * Returns:
  *   1 if "&" is found (background execution requested); 0 otherwise.
  */
 int runBackground(char **args) {
     int i = 0;
     while (args[i] != NULL) {
         if (!strcmp(args[i], "&")) {
             args[i] = NULL;  // Remove the "&" token from the arguments
             return 1;
         }
         i++;
     }
     return 0;
 }
 
 /**
  * executeExecute:
  *   Executes a command based on the tokenized arguments.
  *   1) Calls check_cmd() to handle the "clear" command.
  *   2) If not handled by check_cmd, it checks if the command matches a built-in command from the commands array.
  *   3) If the command is not built-in, it assembles the command into a single string and executes it via system().
  *
  * Parameters:
  *   args: The tokenized command-line arguments.
  *   bg: Background execution flag (ignored here; a warning is printed if set).
  *
  * Note:
  *   Background execution is not supported in this single-process implementation.
  */
 void executeExecute(char **args, int bg) {
     // First, check if the command is "clear" and handle it.
     if (check_cmd(args)) {
         return;
     }
 
     // Warn the user if background execution is requested.
     if (bg == 1) {
         printf("Warning: Background execution not supported in single-process mode. Running in foreground.\n");
     }
 
     // Check if the command matches any built-in command.
     int found = 0;
     for (int i = 0; i < cmdLength(); i++) {
         if (!strcmp(args[0], commands[i].name)) {
             commands[i].func(args);  // Execute the built-in command function.
             found = 1;
             break;
         }
     }
 
     // If the command is not built-in, assemble it into a string and execute it externally using system().
     if (!found) {
         char commandLine[1024] = "";
         for (int i = 0; args[i] != NULL; i++) {
             strcat(commandLine, args[i]);
             if (args[i + 1] != NULL) {
                 strcat(commandLine, " ");
             }
         }
         int ret = system(commandLine);
         if (ret == -1) {
             perror("system() failed");
         }
     }
 }
 
 // ---------------------------------------------------------------------------
 // Built-in Command Implementations
 // ---------------------------------------------------------------------------
 /**
  * kShellHelp:
  *   Displays help information about the built-in commands.
  *
  * Parameters:
  *   args: Tokenized command-line arguments (unused in this function).
  */
 void kShellHelp(char **args) {
     printf("\nMy Shell Help Manual\n");
     printf("--------------------\n");
     printf("cd [directory]   : Change the current directory. If no directory is provided, displays the current directory.\n");
     printf("clr              : Clear the terminal screen (using system(\"clear\")).\n");
     printf("clear            : Clear the terminal screen (using ANSI escape codes, see check_cmd function).\n");
     printf("dir [directory]  : List the contents of a directory.\n");
     printf("environ          : Display all environment variables.\n");
     printf("echo [text]      : Display the provided text on the screen.\n");
     printf("help             : Display this help information.\n");
     printf("pause            : Pause the shell until Enter is pressed.\n");
     printf("quit             : Exit the shell.\n\n");
 }
 
 /**
  * kShellClear:
  *   Clears the terminal screen by invoking system("clear") and prints a confirmation message.
  *
  * Parameters:
  *   args: Tokenized command-line arguments (unused in this function).
  *
  * Note:
  *   This built-in command is associated with the "clr" command.
  */
 void kShellClear(char **args) {
     system("clear");
     printf("Cleared shell (via system(\"clear\"))\n");
 }
 
 /**
  * kShellDir:
  *   Lists the contents of a directory using the "ls -al" command.
  *   If a directory is specified as an argument, it lists that directory; otherwise, it lists the current directory.
  *
  * Parameters:
  *   args: Tokenized command-line arguments.
  */
 void kShellDir(char **args) {
     char input[100] = "ls -al ";
     if (args[1]) {
         strcat(input, args[1]);
     }
     system(input);
 }
 
 /**
  * kShellEcho:
  *   Prints the provided text arguments to the terminal in cyan color.
  *
  * Parameters:
  *   args: Tokenized command-line arguments. The command name "echo" is skipped.
  */
 void kShellEcho(char **args) {
     printf("%s", CYN);  // Set terminal text color to cyan.
     char **arg = args + 1;  // Skip the "echo" command.
     while (*arg) {
         printf("%s ", *arg);
         arg++;
     }
     printf("%s\n", WHT);  // Reset terminal text color to white and add a newline.
 }
 
 /**
  * kShellCD:
  *   Changes the current working directory to the specified directory.
  *   If no directory is provided, prints the current working directory.
  *
  * Parameters:
  *   args: Tokenized command-line arguments.
  */
 void kShellCD(char **args) {
     char cwd[512];
     if (args[1] == NULL) {
         // No directory specified; print the current working directory.
         if (getcwd(cwd, sizeof(cwd)) != NULL) {
             printf("%s\n", cwd);
         } else {
             perror("getcwd failed");
         }
     } else {
         // Attempt to change to the specified directory.
         if (chdir(args[1]) != 0) {
             perror("chdir failed");
         }
     }
 }
 