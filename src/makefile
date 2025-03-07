CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = myshell

all: $(TARGET)

$(TARGET): myshell.o utility.o
	$(CC) $(CFLAGS) -o $(TARGET) myshell.o utility.o

myshell.o: myshell.c utility.h
	$(CC) $(CFLAGS) -c myshell.c

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c utility.c

clean:
	rm -f $(TARGET) *.o
