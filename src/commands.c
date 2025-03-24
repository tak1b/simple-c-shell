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
 *   implemented using ANSI escape codes to clear the screen. Prompt reprinting is handled in myshell.c.
 *
 *   The method for clearing the console using ANSI escape codes is based on an article from GeeksforGeeks:
 *   "Clear the console in C language" (https://www.geeksforgeeks.org/clear-console-c-language/).
 *
 * Note:
 *   This implementation does not fork a child process; external commands are executed using system().
 */

 #include "commands.h"
 #include <unistd.h>  // For getcwd()
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 // ---------------------------------------------------------------------------
 // Static Built-in Command Array
 // ---------------------------------------------------------------------------
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
 /*
  static int cmdLength() {
     return sizeof(commands) / sizeof(commands[0]);
 }
 */

 // ---------------------------------------------------------------------------
 // Additional Function: check_cmd
 // ---------------------------------------------------------------------------
 /**
  * check_cmd:
  *   Checks if the command entered by the user is "clear". If so, it clears the screen using ANSI escape codes.
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
         printf("\e[1;1H\e[2J");
         return 1;  // Command handled
     }
     return 0;      // Not the "clear" command
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
/**
 * executeExecute:
 *   Executes a command, supporting built-in, external, I/O redirection, and background execution.
 *
 * Parameters:
 *   args: Tokenized command-line arguments.
 *   bg: Background execution flag.
 */
void executeExecute(char **args, int bg) {
    if (args[0] == NULL) return;

    if (check_cmd(args)) return;

    // Handle internal commands with redirection
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (!strcmp(args[0], commands[i].name)) {
            pid_t pid = fork();
            if (pid == 0) {
                handleRedirection(args);         // Apply I/O redirection
                commands[i].func(args);          // Execute internal command
                exit(0);                         // Exit child process
            } else {
                waitpid(pid, NULL, 0);           // Wait for child
            }
            return;
        }
    }

    // External command execution
    pid_t pid = fork();
    if (pid == 0) {
        setenv("parent", getenv("shell"), 1);
        handleRedirection(args);                 // Apply I/O redirection
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (!bg) {
            waitpid(pid, NULL, 0);               // Wait for foreground process
        } else {
            printf("Background process started with PID %d\n", pid);
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
    int ret = system("more ../manual/readme.txt");
    if (ret == -1) {
        perror("Failed to open help file using more");
    }
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
     char command[256];
     if (args[1]) {
         snprintf(command, sizeof(command), "ls -al %s", args[1]);
     } else {
         snprintf(command, sizeof(command), "ls -al");
     }
     system(command);
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
 
 /**
 * handleRedirection:
 *   Handles input/output redirection symbols (<, >, >>) in the command arguments.
 */
void handleRedirection(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i+1], O_RDONLY);
            if (fd < 0) { perror("Input file error"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("Output file error"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror("Append file error"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}
