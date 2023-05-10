#  Project 2 of COMP389: Cryptography
#  Function: Makefile
#  File Name: Makefile

CC = gcc
# -Wno-deprecated-declarations is needed to not throw warnings because MD5 is 
#  deprecated as an insecure algorithm in OpenSSL
CFLAGS = -Wall -Wextra -g -Wno-deprecated-declarations
SUPPORT = Makefile README
OBJS := proj2.o
OBJS += pbm.o
OBJS += stream_cipher.o
OBJS += encrypt.o
OBJS += bit_writer.o
OBJS += fns.o

all: proj2

proj2: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -L/c/OpenSSL/lib -lssl -lcrypto -o proj2

stream_cipher.o: stream_cipher.c stream_cipher.h
	$(CC) $(CFLAGS) -c stream_cipher.c

bit_writer.o: bit_writer.c bit_writer.h
	$(CC) $(CFLAGS) -c bit_writer.c

pbm.o: pbm.c pbm.h
	$(CC) $(CFLAGS) -c pbm.c

proj2.o: proj2.c
	$(CC) $(CLAGS) -c proj2.c

encrypt.o: encrypt.c pbm.h
	$(CC) $(CFLAGS) -c encrypt.c

fns.o: fns.c fns.h
	$(CC) $(CFLAGS) -c fns.c

.PHONY: clean, package
clean: 
	rm *.o proj2

package:
	tar -cf *.c *.h $(SUPPORT)
