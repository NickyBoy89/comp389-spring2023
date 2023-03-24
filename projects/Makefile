PROJS := proj1
PROJS += proj2

CC=gcc
CFLAGS=-O1 -Wall

all: proj1 proj2

run: all
	./proj1

test:
	python3 testcases.py

proj1:
	$(CC) $(CFLAGS) proj1.c -o proj1

proj2:
	$(CC) $(CFLAGS) proj2.c -o proj2

clean:
	-rm $(PROJS)
