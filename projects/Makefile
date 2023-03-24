PROJS := proj1

CC=gcc
CFLAGS=-O1 -Wall

all: proj1

run: all
	./proj1

test:
	python3 testcases.py

proj1:
	$(CC) $(CFLAGS) proj1.c -o proj1

clean:
	-rm $(PROJS)
