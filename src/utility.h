/*
 * File: utility.h
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This header file contains the implementations of the built-in shell commands and utility functions.
 *   It defines a structure for mapping command names to function pointers and implements functions
 *   for executing commands, handling I/O redirection, and supporting background execution.
 */

 #ifndef UTILITY_H
 #define UTILITY_H
 
 #include "myshell.h"  // Include the shell header for shared definitions
 
 // Structure that maps built-in command names to their corresponding functions
 struct command {
     char* name;                    // Name of the built-in command (e.g., "clr", "dir")
     void (*func)(char **args);     // Pointer to the function implementing the command
 };
 
 // Array of built-in commands with their associated function pointers
 struct command commands[] = {
     {"clr", kShellClear},  // 'clr' command: clears the screen
     {"dir", kShellDir},    // 'dir' command: lists directory contents
     {"echo", kShellEcho},  // 'echo' command: prints text to the terminal
     {"help", kShellHelp}   // 'help' command: displays the help manual
 };
 
 /**
  * cmdLength:
  *   Returns the total number of built-in commands available in the shell.
  *
  * Returns: Integer count of built-in commands.
  */
 int cmdLength() {
     return sizeof(commands) / sizeof(struct command);
 }
 
 /**
  * runBackground:
  *   Scans the argument list for the "&" token, indicating that the command should be executed
  *   in the background. If found, the "&" token is removed from the arguments.
  *
  * args: Array of command arguments.
  *
  * Returns: 1 if background execution is requested, 0 otherwise.
  */
 int runBackground(char **args) {
     int i = 0;
     while (args[i] != NULL) {
         if (strcmp(args[i], "&") == 0) {
             args[i] = NULL;  // Remove the background token
             return 1;        // Indicate background execution
         }
         i++;
     }
     return 0;
 }
 
 /**
  * executeExecute:
  *   Executes the provided command after checking for input/output redirection operators.
  *   It forks a child process in which the command is executed. If the command is built-in,
  *   its corresponding function is invoked; otherwise, execvp is used to run external commands.
  *
  * args: Array of command arguments.
  * bg: Flag for background execution (1 for background, 0 for foreground).
  */
 void executeExecute(char **args, int bg) {
     int inputRed = 0;    // Flag for input redirection
     int outputRed = 0;   // Flag for output redirection (overwrite mode)
     int appendRed = 0;   // Flag for output redirection (append mode)
     char *input_file = NULL;   // Filename for input redirection
     char *outputFile = NULL;   // Filename for output redirection
     
     // Count the number of arguments provided
     int count = 0;
     while (args[count] != NULL) {
         count++;
     }
     
     // Scan arguments for redirection operators and set flags accordingly
     for (int i = 0; i < count; i++) {
         if (strcmp(args[i], "<") == 0) {
             inputRed = 1;
             input_file = args[i + 1];  // Next argument is the input file name
             args[i] = NULL;            // Remove operator from arguments
         } else if (strcmp(args[i], ">") == 0) {
             outputRed = 1;
             outputFile = args[i + 1];  // Next argument is the output file name
             args[i] = NULL;            // Remove operator from arguments
         } else if (strcmp(args[i], ">>") == 0) {
             appendRed = 1;
             outputFile = args[i + 1];  // Next argument is the output file name
             args[i] = NULL;            // Remove operator from arguments
         }
     }
     
     // Fork a child process to execute the command
     pid_t pid = fork();
     if (pid == -1) {
         perror("Fork failed");
         exit(EXIT_FAILURE);
     }
     
     if (pid == 0) {  // Child process
         // Handle input redirection if specified
         if (inputRed) {
             int fd = open(input_file, O_RDONLY);
             if (fd == -1) {
                 perror("Failed to open input file");
                 exit(EXIT_FAILURE);
             }
             if (dup2(fd, STDIN_FILENO) == -1) {
                 perror("Duplication of file descriptor failed");
                 exit(EXIT_FAILURE);
             }
             close(fd);  // Close file descriptor after redirection
         }
         
         // Handle output redirection if specified
         if (outputRed || appendRed) {
             int flags = O_WRONLY | O_CREAT;
             flags |= (appendRed) ? O_APPEND : O_TRUNC;  // Set appropriate flag
             int fd = open(outputFile, flags, 0644);
             if (fd == -1) {
                 perror("Failed to open output file");
                 exit(EXIT_FAILURE);
             }
             if (dup2(fd, STDOUT_FILENO) == -1) {
                 perror("Duplication of file descriptor failed");
                 exit(EXIT_FAILURE);
             }
             close(fd);  // Close file descriptor after redirection
         }
         
         // Check if the command is a built-in command and execute it if so
         for (int i = 0; i < cmdLength(); i++) {
             if (strcmp(args[0], commands[i].name) == 0) {
                 commands[i].func(args);  // Call the built-in function
                 exit(EXIT_SUCCESS);      // Exit child process after execution
             }
         }
         
         // For non-built-in commands, execute the external command using execvp
         execvp(args[0], args);
         // If execvp returns, an error occurred
         perror("Execution error");
         exit(EXIT_FAILURE);
     } else {  // Parent process
         int status;
         if (bg == 1) {
             // For background processes, print the process ID and wait for it (non-blocking)
             printf("Process [%d]\n", pid);
             waitpid(pid, &status, WUNTRACED);
         } else {
             // For foreground processes, wait until the child process terminates
             do {
                 if (waitpid(pid, &status, WUNTRACED) == -1) {
                     perror("An error occurred when waiting for process");
                     exit(EXIT_FAILURE);
                 }
             } while (!WIFEXITED(status) && !WIFSIGNALED(status));
         }
     }
 }
 
 /**
  * kShellHelp:
  *   Displays the help manual to the user. It changes the current directory to locate the
  *   manual file (readme.md) and then invokes the system's "more" command to display it.
  *
  * args: Array of command arguments (unused in this function).
  */
 void kShellHelp(char **args) {
     // Change directory to one level up, then to the "manual" folder
     chdir("..");
     chdir("manual");
     // Use the "more" command with the -p flag to paginate the readme.md file
     system("more -p readme.md");
     printf("\n\n");
     // Exit the child process after displaying the help manual
     exit(1);
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
  *   Prints the provided text to the terminal. It skips the command name ("echo") and
  *   prints the remaining arguments.
  *
  * args: Array of command arguments.
  */
 void kShellEcho(char **args) {
     // Set the text color to cyan for output
     printf("%s", CYN);
     // Start printing arguments from the second element
     char **arg = args + 1;
     while (*arg) {
         printf("%s ", *arg);
         arg++;
     }
     // Reset the text color and print a newline
     printf("%s\n", WHT);
 }
 
 /**
  * kShellCD:
  *   Changes the current working directory. If no argument is provided, changes to the
  *   home directory. Optionally, it lists the contents of the new directory.
  *
  * args: Array of command arguments.
  */
 void kShellCD(char **args) {
     if (args[1] == NULL) {
         // If no directory is specified, change to the home directory
         chdir(getenv("HOME"));
     } else {
         // Change to the directory provided in args[1]
         chdir(args[1]);
     }
     // Optionally, list the directory contents to confirm the change
     system("ls");
     printf("%s", WHT);
 }
 
 /**
  * kShellDir:
  *   Lists the contents of a directory by constructing an "ls -al" command.
  *   If an argument is provided, that directory is listed; otherwise, the current directory is listed.
  *
  * args: Array of command arguments.
  */
 void kShellDir(char **args) {
     char input[100] = "ls -al ";  // Base command for detailed directory listing
     if (args[1]) {
         // Append the specified directory to the command string
         strcat(input, args[1]);
     }
     // Execute the command using the system() call
     system(input);
 }
 
 #endif  // End of UTILITY_H
 