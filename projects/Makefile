PROJS := proj1
PROJS += proj2

OBJS := pbm.o

CC=clang
CFLAGS=-O1 -Wall

all: proj1 proj2

run: all
	./proj1

test:
	python3 testcases.py

proj1: proj1.c
	$(CC) $(CFLAGS) proj1.c -o proj1

proj2: proj2.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) proj2.c -o proj2

proj2.o: proj2.c

pbm.o: pbm.c pbm.h
	$(CC) $(CFLAGS) -c pbm.c

clean:
	-rm $(PROJS)
	-rm $(OBJS)
