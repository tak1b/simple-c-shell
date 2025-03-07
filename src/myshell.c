/*
 * File: myshell.c
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This file contains the main function and command processing loop for the Custom C Shell.
 *   It supports both interactive mode and batch mode (reading commands from a file). The shell
 *   displays a prompt with the current working directory and processes built-in commands.
 *   In this update, if the "cd" command is issued without an argument, it simply prints the
 *   current working directory.
 */

 #include "myshell.h"      // Include header with macros and function prototypes
 #include "utility.h"      // Include built-in command implementations
 
 int main (int argc, char *argv[], char *envp[]) {
     char buf[MAX_BUFFER];         // Buffer to store user input
     char *args[MAX_ARGS];         // Array to store tokenized command arguments
     char **arg;                   // Pointer to iterate through args array
     char cwd[500];                // Buffer to store the current working directory
     int dontShowPrompt = 0;       // Flag to control prompt display after executing a command
     int batchmode = 0;            // Flag to indicate whether shell is in batch mode
 
     // Set the "shell" environment variable to the full path of the executable
     char fullPath[1024];
     if (realpath(argv[0], fullPath) != NULL) {
         setenv("shell", fullPath, 1);
     }
 
     // Check if a batch file argument was provided and if it exists
     if (argc > 1 && access(argv[1], F_OK) == 0) {
         batchmode = 1;
     }
     
     // Interactive mode: no batch file provided
     if (!batchmode) {
         // Build a prompt string containing the shell identifier and current working directory
         char prompt[50] = "MS ";
         strcat(prompt, getcwd(cwd, sizeof(cwd)));  // Append current directory
         strcat(prompt, " $~ ");
         
         // Print startup messages
         printf("Now Running\n");
         printf("%sMy Shell\n\n", MAG);
         printf("%s\n", WHT);
 
         // Main interactive loop: read commands until end-of-file (Ctrl-D) or "quit" command
         while (!feof(stdin)) {
             // Display the prompt if needed
             if (dontShowPrompt == 1) {
                 dontShowPrompt = 0;
             } else {
                 prompt[0] = '\0';  // Reset prompt string
                 strcat(prompt, "MS ");
                 strcat(prompt, getcwd(cwd, sizeof(cwd)));
                 strcat(prompt, " $~ ");
                 fputs(prompt, stdout);  // Output the prompt
             }
             
             // Read a line of input from the user
             if (fgets(buf, MAX_BUFFER, stdin)) {
                 // Tokenize the input string using defined separators (space, tab, newline)
                 arg = args;
                 *arg++ = strtok(buf, SEPARATORS);  // First token (command)
                 while ((*arg++ = strtok(NULL, SEPARATORS)));  // Subsequent tokens
 
                 // Process the command if not empty
                 if (args[0]) {
                     // Check for background execution symbol ("&")
                     int background = runBackground(args);
 
                     // Process built-in commands using string comparisons
                     if (!strcmp(args[0],"pause")) {
                         // For "pause", wait for the user to press Enter
                         printf("Pausing, press enter to continue\n\n");
                         getchar();
                         continue;
                     } else if (!strcmp(args[0],"environ")) {
                         // For "environ", print all environment variables
                         for (int i = 0; envp[i] != NULL; i++) {
                             printf("%s\n", envp[i]);
                         }
                         printf("\n");
                     } else if (!strcmp(args[0],"quit")) {
                         // For "quit", exit the shell loop
                         break;
                     } else if (!strcmp(args[0],"cd")) {
                         // If cd is given without an argument, print the current directory (pwd)
                         if (args[1] == NULL) {
                             if (getcwd(cwd, sizeof(cwd)) != NULL) {
                                 printf("%s\n", cwd);
                             } else {
                                 perror("getcwd failed");
                             }
                         } else {
                             // Otherwise, attempt to change to the specified directory
                             if (chdir(args[1]) != 0) {
                                 perror("chdir failed");
                             }
                         }
                     } else {
                         // For any other command, execute using the executeExecute function
                         if (background == 1) {
                             executeExecute(args, background);
                         } else {
                             dontShowPrompt = 1;  // Set flag to avoid duplicate prompt display
                             executeExecute(args, background);
                         }
                     }
                 }
             }
         }
     } else {
         // Batch mode: process commands from a file specified as a command-line argument
         FILE *file = fopen(argv[1], "r");  // Open the batch file for reading
         if (file == NULL) {
             perror("Error opening file");
             return 1;
         }
         
         // Arrays and counters for reading and processing file lines
         char line[MAX_LINE_LENGTH];               // Buffer for each line
         char lines[1000][MAX_LINE_LENGTH];          // Array to store multiple lines
         char *tokens[MAX_TOKENS];                   // Array for tokenized words in each line
         int tokenCount;                             // Counter for tokens in a line
         int lineCount = 0;                          // Number of lines read from file
         int i = 0;
         
         // Read each line from the file and store in the lines array
         while (fgets(line, sizeof(line), file) != NULL) {
             strcpy(lines[i], line);
             lineCount++;
             i++;
         }
         i = 0;
         // Process each stored line from the batch file
         while (i < lineCount) {
             tokenCount = 0;  // Reset token count for current line
             char *token = strtok(lines[i], SEPARATORS);  // Tokenize the line
             while (token != NULL && tokenCount < MAX_TOKENS) {
                 tokens[tokenCount++] = token;
                 token = strtok(NULL, SEPARATORS);
             }
             // Execute the command if the line is not empty
             if (tokens[0]) {
                 int background = runBackground(tokens);
                 if (!strcmp(tokens[0],"pause")) {
                     printf("Pausing, press enter to continue\n\n");
                     getchar();
                     continue;
                 } else if (!strcmp(tokens[0],"environ")) {
                     for (int i = 0; envp[i] != NULL; i++) {
                         printf("%s\n", envp[i]);
                     }
                     printf("\n");
                 } else if (!strcmp(tokens[0],"quit")) {
                     break;
                 } else if (!strcmp(tokens[0],"cd")) {
                     // In batch mode, if cd has no argument, print the current directory
                     if (tokens[1] == NULL) {
                         if (getcwd(cwd, sizeof(cwd)) != NULL) {
                             printf("%s\n", cwd);
                         } else {
                             perror("getcwd failed");
                         }
                     } else {
                         if (chdir(tokens[1]) != 0) {
                             perror("chdir failed");
                         }
                     }
                 } else {
                     executeExecute(tokens, background);
                 }
             }
             i++;
             // Clear the tokens array for the next iteration
             for (int j = 0; j < MAX_TOKENS; j++) {
                 tokens[j] = NULL;
             }
         }
         // Close the batch file after processing all commands
         fclose(file);
     }
     return 0;  // Exit the shell successfully
 }
 