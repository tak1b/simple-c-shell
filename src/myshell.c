/*
 * File: myshell.c
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This is the main file for the Custom C Shell. It contains the main() function and the main
 *   command processing loop. The shell can operate in both interactive mode and batch mode.
 *
 *   In interactive mode, the shell displays a prompt that includes the current working directory,
 *   reads user input, tokenizes it, and then processes built-in or external commands.
 *
 *   In batch mode, the shell reads commands from a file provided as a command-line argument.
 *
 *   Additionally, the shell sets an environment variable "shell" to the full path of the executable.
 */

 #include "myshell.h"    // Common definitions and macros
 #include "commands.h"   // Declarations for built-in command and utility functions
 
 int main(int argc, char *argv[], char *envp[]) {
     char buf[MAX_BUFFER];   // Buffer to store user input
     char *args[MAX_ARGS];   // Array to store tokenized command arguments
     char cwd[500];          // Buffer to store current working directory
     int batchmode = 0;      // Flag to determine whether to run in batch mode (reading commands from a file)
 
     // Set the "shell" environment variable to the full path of the executable
     char fullPath[1024];
     if (realpath(argv[0], fullPath) != NULL) {
         setenv("shell", fullPath, 1);
     }
 
     // Determine if batch mode should be enabled by checking for a valid file argument
     if (argc > 1 && access(argv[1], F_OK) == 0) {
         batchmode = 1;
     }
 
     if (!batchmode) {
         // ---------------------------
         // Interactive Mode
         // ---------------------------
         char prompt[1024];
 
         // Print initial startup messages
         printf("Now Running\n");
         printf("%sMy Shell\n\n", MAG);
         printf("%s", WHT);
 
         while (1) {
             // Build and print the prompt every time
             prompt[0] = '\0';
             strcat(prompt, "MS ");
             strcat(prompt, getcwd(cwd, sizeof(cwd)));
             strcat(prompt, " $~ ");
             fputs(prompt, stdout);
             fflush(stdout);
 
             // Read a line of input from the user; break on EOF (Ctrl+D)
             if (fgets(buf, MAX_BUFFER, stdin) == NULL) {
                 break;
             }
 
             // Tokenize the input
             char **argPtr = args;
             *argPtr++ = strtok(buf, SEPARATORS);
             while ((*argPtr++ = strtok(NULL, SEPARATORS)));
 
             // Process the command if the user typed something
             if (args[0]) {
                 int background = runBackground(args);  // Check for "&"
 
                 if (!strcmp(args[0], "pause")) {
                     // Pause the shell until the user presses Enter
                     printf("Pausing, press enter to continue\n\n");
                     getchar();
                     continue;
                 } else if (!strcmp(args[0], "environ")) {
                     // Print all environment variables
                     for (int i = 0; envp[i] != NULL; i++) {
                         printf("%s\n", envp[i]);
                     }
                     printf("\n");
                 } else if (!strcmp(args[0], "quit")) {
                     // Exit the shell if the "quit" command is given
                     break;
                 } else if (!strcmp(args[0], "cd")) {
                     // Handle the "cd" command
                     kShellCD(args);
                 } else {
                     // For all other commands, use executeExecute to process them
                     executeExecute(args, background);
                 }
             }
         }
     } else {
         // ---------------------------
         // Batch Mode
         // ---------------------------
         FILE *file = fopen(argv[1], "r");
         if (!file) {
             perror("Error opening batch file");
             return 1;
         }
 
         char line[MAX_LINE_LENGTH];
         char lines[1000][MAX_LINE_LENGTH];
         char *tokens[MAX_TOKENS];
         int lineCount = 0;
         int i = 0;
 
         // Read all lines from the batch file
         while (fgets(line, sizeof(line), file) != NULL && lineCount < 1000) {
             strcpy(lines[lineCount], line);
             lineCount++;
         }
         fclose(file);
 
         // Process each line
         while (i < lineCount) {
             int tokenCount = 0;
             char *token = strtok(lines[i], SEPARATORS);
             while (token != NULL && tokenCount < MAX_TOKENS) {
                 tokens[tokenCount++] = token;
                 token = strtok(NULL, SEPARATORS);
             }
             tokens[tokenCount] = NULL;
 
             // If the line is not empty, process the command
             if (tokens[0]) {
                 int background = runBackground(tokens);
 
                 if (!strcmp(tokens[0], "pause")) {
                     printf("Pausing, press enter to continue\n\n");
                     getchar();
                 } else if (!strcmp(tokens[0], "environ")) {
                     for (int j = 0; envp[j] != NULL; j++) {
                         printf("%s\n", envp[j]);
                     }
                     printf("\n");
                 } else if (!strcmp(tokens[0], "quit")) {
                     break;  // Exit batch mode on "quit" command
                 } else if (!strcmp(tokens[0], "cd")) {
                     kShellCD(tokens);
                 } else {
                     executeExecute(tokens, background);
                 }
             }
             i++;
         }
     }
 
     return 0;  // Exit the shell with a successful return code
 }
 