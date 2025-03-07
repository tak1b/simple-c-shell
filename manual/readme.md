# My Shell User Manual

## Overview
This shell is a simple command-line interpreter built in C. It supports basic internal commands
and can also run external commands.

## Internal Commands

### 1. cd
**Usage:** `cd [directory]`  
Changes the current working directory. If no directory is specified, it defaults to the home directory.

### 2. clr
**Usage:** `clr`  
Clears the terminal screen.

### 3. dir
**Usage:** `dir [directory]`  
Lists the contents of the specified directory (or the current directory if none is provided).

### 4. environ
**Usage:** `environ`  
Displays all the environment variables.

### 5. echo
**Usage:** `echo [text]`  
Displays the provided text on the screen.

### 6. help
**Usage:** `help`  
Displays this manual.

### 7. pause
**Usage:** `pause`  
Pauses the shell until the user presses Enter.

### 8. quit
**Usage:** `quit`  
Exits the shell.

## Usage Instructions
- **Interactive Mode:** Type `./myshell` to launch, and enter commands at the prompt.
- **Batch Mode:** Type `./myshell batchfile` to read commands from a file.
- **Redirection:** Use `<`, `>`, `>>` for input and output redirection (Stage 2+).
- **Background Execution:** Append `&` to run a command in the background (Stage 2+).

