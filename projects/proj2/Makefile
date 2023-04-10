PROJS := proj1
PROJS += proj2

PROJ1_OBJS := base64.o
PROJ2_OBJS := pbm.o

CC=gcc
CFLAGS=-O1 -Wall -Wextra -pedantic

all: $(PROJS)

test:
	python3 testcases.py

proj1: proj1.c $(PROJ1_OBJS)
	$(CC) $(CFLAGS) $(PROJ1_OBJS) proj1.c -o proj1

base64.o: base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c

proj2: proj2.c $(PROJ2_OBJS)
	$(CC) $(CFLAGS) $(PROJ2_OBJS) proj2.c -o proj2

pbm.o: pbm.c pbm.h
	$(CC) $(CFLAGS) -c pbm.c

clean:
	-rm $(PROJS)
	-rm $(PROJ1_OBJS)
	-rm $(PROJ2_OBJS)
