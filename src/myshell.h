/*
 * File: myshell.h
 * Author: Takib Islam
 * Student No: 23339236
 * Title: Custom C Shell -- My Shell
 * I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.
 *
 * Description:
 *   This header file contains common includes, macro definitions, and constants used by the shell.
 *   It is included by both the main shell (myshell.c) and the commands implementation file (commands.c).
 *
 * Note:
 *   This file does not include any function implementations.
 */

 #ifndef MY_SHELL_H
 #define MY_SHELL_H
 
 // Standard library includes for input/output, string handling, process control, etc.
 #include <stdio.h>      // For standard input/output functions (printf, perror, etc.)
 #include <stdlib.h>     // For general functions (exit, getenv, setenv, etc.)
 #include <string.h>     // For string manipulation functions (strcpy, strcat, strcmp, strtok, etc.)
 #include <unistd.h>     // For POSIX functions (chdir, getcwd, access, etc.)
 #include <fcntl.h>      // For file control options (open, O_RDONLY, etc.)
 #include <sys/types.h>  // For data types used in system calls (pid_t, etc.)
 #include <sys/wait.h>   // For waiting on process termination (wait, waitpid)
 #include <dirent.h>     // For directory operations (opendir, readdir)
 #include <stdbool.h>    // For boolean type (bool, true, false)
 #include <signal.h>     // For signal handling functions
 
 // ANSI escape codes for terminal text colors
 #define BLK "\e[0;30m"   // Black
 #define RED "\e[0;31m"   // Red
 #define GRN "\e[0;32m"   // Green
 #define YEL "\e[0;33m"   // Yellow
 #define BLU "\e[0;34m"   // Blue
 #define MAG "\e[0;35m"   // Magenta
 #define CYN "\e[0;36m"   // Cyan
 #define WHT "\e[0;37m"   // White
 #define reset "\e[0m"    // Reset to default
 
 // Buffer sizes and limits used in the shell
 #define MAX_BUFFER 1024          // Maximum characters for input buffer
 #define MAX_LINE_LENGTH 1024     // Maximum length for a line read from a file (batch mode)
 #define MAX_ARGS 64              // Maximum number of arguments per command
 #define SEPARATORS " \t\n"       // Delimiters used for tokenizing input (space, tab, newline)
 #define MAX_TOKENS 50            // Maximum number of tokens that can be parsed from a command
 
 #endif // MY_SHELL_H
 