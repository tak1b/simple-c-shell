# Makefile for CustomShell - Stage 2
# Compiles the shell and places binary in ../bin

CC = gcc
CFLAGS = -Wall
TARGET = ../bin/customshell

SRCS = myshell.c commands.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)
