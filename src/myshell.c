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
     char cwd[500];          // Buffer to store current working directory for prompt
     int dontShowPrompt = 0; // Flag used to control prompt display after executing a command
     int batchmode = 0;      // Flag to determine whether to run in batch mode (reading commands from a file)
 
     // Set the "shell" environment variable to the full path of the executable
     // realpath(argv[0], fullPath) obtains the canonicalized absolute pathname of the shell
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
         // Construct the initial prompt string
         char prompt[50] = "MS ";
         strcat(prompt, getcwd(cwd, sizeof(cwd)));  // Append the current working directory to the prompt
         strcat(prompt, " $~ ");
 
         // Print initial startup messages
         printf("Now Running\n");
         printf("%sMy Shell\n\n", MAG);
         printf("%s", WHT);
 
         // Main interactive loop: keep reading user input until EOF (Ctrl-D) or "quit" command is encountered
         while (!feof(stdin)) {
             if (dontShowPrompt == 1) {
                 dontShowPrompt = 0;  // Reset the flag if it was previously set
             } else {
                 // Rebuild the prompt to reflect any changes in the current directory
                 prompt[0] = '\0';   // Reset prompt string
                 strcat(prompt, "MS ");
                 strcat(prompt, getcwd(cwd, sizeof(cwd)));
                 strcat(prompt, " $~ ");
                 fputs(prompt, stdout);  // Output the prompt to the terminal
             }
 
             // Read a line of input from the user
             if (fgets(buf, MAX_BUFFER, stdin)) {
                 // Tokenize the input using defined separators (space, tab, newline)
                 char **argPtr = args;  // Pointer to fill the args array
                 *argPtr++ = strtok(buf, SEPARATORS);  // First token (command)
                 while ((*argPtr++ = strtok(NULL, SEPARATORS)));  // Subsequent tokens; array is terminated with NULL
 
                 // Process the command if the user entered something (args[0] is not NULL)
                 if (args[0]) {
                     int background = runBackground(args);  // Check for background execution token ("&")
 
                     // Handle specific built-in commands that are processed in the main loop
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
                         // Handle the "cd" command (see kShellCD in commands.c for details)
                         kShellCD(args);
                     } else {
                         // For all other commands, use executeExecute to process them
                         dontShowPrompt = 1;  // Prevent immediate reprinting of the prompt
                         executeExecute(args, background);
                     }
                 }
             }
         }
     } else {
         // ---------------------------
         // Batch Mode
         // ---------------------------
         // Open the batch file for reading commands
         FILE *file = fopen(argv[1], "r");
         if (!file) {
             perror("Error opening batch file");
             return 1;
         }
 
         char line[MAX_LINE_LENGTH];    // Buffer for each line from the batch file
         char lines[1000][MAX_LINE_LENGTH]; // Array to store lines (up to 1000)
         char *tokens[MAX_TOKENS];      // Array for tokenized arguments from each line
         int lineCount = 0;             // Counter for the number of lines read from the file
         int i = 0;
 
         // Read all lines from the batch file into the lines array
         while (fgets(line, sizeof(line), file) != NULL) {
             strcpy(lines[lineCount], line);
             lineCount++;
         }
         fclose(file);  // Close the file after reading
 
         // Process each line from the batch file
         while (i < lineCount) {
             int tokenCount = 0;
             // Tokenize the current line
             char *token = strtok(lines[i], SEPARATORS);
             while (token != NULL && tokenCount < MAX_TOKENS) {
                 tokens[tokenCount++] = token;
                 token = strtok(NULL, SEPARATORS);
             }
             tokens[tokenCount] = NULL;  // Ensure the tokens array is NULL-terminated
 
             // If the line is not empty, process the command
             if (tokens[0]) {
                 int background = runBackground(tokens);  // Check for "&" in the tokens
 
                 // Handle specific commands in batch mode
                 if (!strcmp(tokens[0], "pause")) {
                     printf("Pausing, press enter to continue\n\n");
                     getchar();
                 } else if (!strcmp(tokens[0], "environ")) {
                     for (int i = 0; envp[i] != NULL; i++) {
                         printf("%s\n", envp[i]);
                     }
                     printf("\n");
                 } else if (!strcmp(tokens[0], "quit")) {
                     break;  // Exit batch mode on "quit" command
                 } else if (!strcmp(tokens[0], "cd")) {
                     kShellCD(tokens);
                 } else {
                     // For all other commands, execute them
                     executeExecute(tokens, background);
                 }
             }
             i++;
         }
     }
 
     return 0;  // Exit the shell with a successful return code
 }
 