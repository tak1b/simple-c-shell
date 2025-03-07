/*
 * File: myshell.h
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This header file provides the necessary include directives, macro definitions, and function
 *   prototypes for the Custom C Shell project (Stage 1). The shell implements basic internal commands
 *   and supports batch mode command file processing.
 */

 #ifndef MY_SHELL_H
 #define MY_SHELL_H
 
 // Standard library includes
 #include <stdio.h>      // Standard input/output functions
 #include <stdlib.h>     // General utilities: memory allocation, process control, conversions, etc.
 #include <string.h>     // String handling functions (e.g., strcpy, strcat, strcmp)
 #include <unistd.h>     // POSIX API functions (e.g., chdir, access)
 #include <fcntl.h>      // File control options (open, O_RDONLY, etc.)
 #include <sys/types.h>  // Data types used in system calls
 #include <sys/wait.h>   // Wait for process termination (waitpid)
 #include <dirent.h>     // Directory traversing functions
 #include <stdbool.h>    // Boolean type (true/false)
 #include <signal.h>     // Signal handling
 
 // ANSI escape codes for terminal colors
 #define BLK "\e[0;30m"   // Black
 #define RED "\e[0;31m"   // Red
 #define GRN "\e[0;32m"   // Green
 #define YEL "\e[0;33m"   // Yellow
 #define BLU "\e[0;34m"   // Blue
 #define MAG "\e[0;35m"   // Magenta
 #define CYN "\e[0;36m"   // Cyan
 #define WHT "\e[0;37m"   // White
 #define reset "\e[0m"    // Reset colors
 
 // Buffer sizes and limits
 #define MAX_BUFFER 1024          // Maximum characters for input buffer
 #define MAX_LINE_LENGTH 1024     // Maximum line length for batch mode input
 #define MAX_ARGS 64              // Maximum number of command arguments
 #define SEPARATORS " \t\n"       // Token delimiters: space, tab, newline
 #define MAX_TOKENS 50            // Maximum number of tokens per line
 
 // Function prototypes for shell operations and built-in commands
 
 /**
  * executeExecute:
  *   Executes a command given as an array of arguments.
  *   Handles input/output redirection and background execution.
  *
  * args: Array of command strings.
  * bg: Background flag (1 if command should run in background, 0 otherwise).
  */
 void executeExecute(char **args, int bg);
 
 /**
  * kShellClear:
  *   Clears the terminal screen by calling the system's "clear" command.
  *
  * args: Array of arguments (not used).
  */
 void kShellClear(char **args);
 
 /**
  * kShellHelp:
  *   Displays the help manual using the system's "more" command.
  *   Navigates to the proper directory to locate the manual file.
  *
  * args: Array of arguments (not used).
  */
 void kShellHelp(char **args);
 
 /**
  * kShellDir:
  *   Lists the contents of a directory. If a directory is provided as an argument,
  *   it lists that directory; otherwise, it lists the current directory.
  *
  * args: Array of command arguments.
  */
 void kShellDir(char **args);
 
 /**
  * kShellEcho:
  *   Prints the provided text to the screen.
  *
  * args: Array of command arguments; args[0] is "echo", and the rest are printed.
  */
 void kShellEcho(char **args);
 
 /**
  * kShellCD:
  *   Changes the current working directory. If no directory is specified,
  *   changes to the home directory.
  *
  * args: Array of command arguments.
  */
 void kShellCD(char **args);
 
 /**
  * runBackground:
  *   Checks for the "&" symbol among the arguments, indicating the command
  *   should be executed in the background. Removes "&" from the argument list.
  *
  * args: Array of command arguments.
  *
  * Returns: 1 if background execution is requested, 0 otherwise.
  */
 int runBackground(char **args);
 
 #endif  // End of MY_SHELL_H
 