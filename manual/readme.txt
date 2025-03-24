My Shell (Custom C Shell)
=========================

Author: Takib Islam
Student No: 23339236

I acknowledge that I have read and agreed to abide by DCU's Academic Integrity Policy.

--------------------------------------------------
Overview
--------------------------------------------------
My Shell is a simple command-line interpreter written in C. It supports a range of
built-in commands for managing directories, displaying text, and handling environment
variables. This manual explains how to build the shell, run it in both interactive
and batch modes, and use its various commands.

--------------------------------------------------
Building the Shell
--------------------------------------------------
To compile the shell, ensure you have a C compiler installed (e.g., gcc) and use the
provided makefile.

1. Open a terminal in the project directory.
2. Run the following command:

   make

This will compile the source files and generate an executable named "myshell".

--------------------------------------------------
Running the Shell
--------------------------------------------------
Interactive Mode:
-----------------
To start the shell in interactive mode, run:

   ./myshell

The shell will display a prompt that includes your current working directory.
You can then type commands directly.

Batch Mode:
-----------
To run the shell in batch mode (commands are read from a file), run:

   ./myshell batchfile

Replace "batchfile" with the name of your file containing a list of commands. The shell
will execute the commands line by line and exit when the end of the file is reached.

--------------------------------------------------
Supported Commands
--------------------------------------------------
cd [directory]
   Changes the current directory to the specified directory. If no directory is provided,
   the shell prints the current working directory.

clr
   Clears the terminal screen using system("clear").

clear
   Clears the terminal screen using ANSI escape sequences and reprints the prompt 
   (showing the current working directory) so that user information remains visible.
   Source: https://www.geeksforgeeks.org/clear-console-c-language/

dir [directory]
   Lists the contents of the specified directory in detail using "ls -al".
   If no directory is specified, it lists the contents of the current directory.

environ
   Displays all the current environment variables.

echo [text]
   Prints the provided text on the screen.

help
   Opens this help manual using the "more" filter. Use the arrow keys to scroll 
   and press "q" to exit.

pause
   Pauses the shell until the user presses Enter.

quit
   Exits the shell.

--------------------------------------------------
Example Usage
--------------------------------------------------
Changing Directory:
   To change to a specific directory:
      cd /home/user/Documents
   To print the current working directory (by typing cd with no arguments):
      cd

Clearing the Screen:
   Using the built-in command that calls system("clear"):
      clr
   Using ANSI escape sequences to clear the screen and reprint the prompt:
      clear

Listing Directory Contents:
      dir /home/user

Displaying Environment Variables:
      environ

Displaying Text:
      echo Hello, World!

Opening the Help Manual:
      help
   (The manual will open using "more". Use arrow keys to scroll and press "q" to exit.)

Pausing the Shell:
      pause

Exiting the Shell:
      quit

--------------------------------------------------
Environment Variable
--------------------------------------------------
At startup, the shell sets an environment variable named "shell" to the full path of the
shell executable. This is determined dynamically at runtime.

--------------------------------------------------
Known Limitations
--------------------------------------------------
- Background execution is not supported in this single-process implementation.
- External commands are executed synchronously via the system() function.

--------------------------------------------------
Maintainers & Contributers
--------------------------------------------------
- Takib Islam - Lead Maintainer

--------------------------------------------------
Credits and Acknowledgments
--------------------------------------------------
- This shell project is based on assignment requirements provided by DCU.
- Special thanks to Dr. Graham Healy for the initial project concept.
- The ANSI escape sequence technique for clearing the screen is adapted from:
  GeeksforGeeks (https://www.geeksforgeeks.org/clear-console-c-language/).
- Thank you to Aman Ahmmed for providing aliases to ANSI Colour Codes

--------------------------------------------------
Reading Material
--------------------------------------------------
- Quigley, E. (2010) Unix shells by example. Upper Saddle River, NJ: Prentice Hall Professional Technical Reference.


--------------------------------------------------
Stage 2 Features
--------------------------------------------------

External Commands:
    You can now execute external commands like 'ls', 'cat', 'gcc', etc.
    Example:
        ls -l
        gcc myprog.c -o myprog

Background Execution:
    Use '&' at the end of a command to run it in the background.
    Example:
        sleep 10 &

Input/Output Redirection:
    <  : Redirect input
    >  : Redirect output (overwrite)
    >> : Redirect output (append)

    Example:
        sort < unsorted.txt > sorted.txt
        echo "done" >> log.txt
