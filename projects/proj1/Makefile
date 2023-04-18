PROJ := proj1

OBJS := base64.o hexdump.o proj1.o

CC=gcc
CFLAGS=-O2 -g -Wall -Wextra

all: $(PROJ)

base64_test: base64.o
	$(CC) $(CFLAGS) base64.o base64_test.c -o base64_test

proj1: proj1.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o proj1

proj1.o: proj1.c
	$(CC) $(CFLAGS) -c proj1.c

base64.o: base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c

hexdump.o: hexdump.c hexdump.h
	$(CC) $(CFLAGS) -c hexdump.c

clean:
	-rm $(PROJ)
	-rm $(OBJS)
