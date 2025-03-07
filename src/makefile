# Makefile for building the My Shell executable.
# Author: Takib Islam, Student No: 23339236
# This Makefile compiles myshell.c and commands.c into a single executable named "myshell".
#
# Usage:
#   - Run 'make' to compile the code.
#   - Run 'make clean' to remove the compiled executable.
#
# Note:
#   Ensure that your source files (myshell.c, myshell.h, commands.h, commands.c) are in the same directory.
#   The gcc command below uses the -Wall flag to enable all compiler warnings for better code quality.

myshell: myshell.c commands.c
		gcc -Wall myshell.c commands.c -o myshell

clean:
		rm -f myshell
