# Makefile for CustomShell - Stage 2
# Located in bin/, compiles ../src/*.c into customshell in this directory

CC = gcc
CFLAGS = -Wall
TARGET = myshell
SRCDIR = ../src

SRCS = $(SRCDIR)/myshell.c $(SRCDIR)/commands.c
OBJS = myshell.o commands.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
