CFLAGS = -Wall -pedantic -std=c99 -DLIST
CC = gcc

all: htest1 htest2 htest3 htest4

htest1: htest1.o hash.c hash.h
	$(CC) $(CFLAGS) htest1.o hash.c -o htest1

htest1.o: htest1.c
	$(CC) $(CFLAGS) -c htest1.c -o htest1.o

htest2: htest2.o hash.c hash.h
	$(CC) $(CFLAGS) htest2.o hash.c -o htest2

htest2.o: htest2.c
	$(CC) $(CFLAGS) -c htest2.c -o htest2.o

htest3: htest3.o hash.c hash.h
	$(CC) $(CFLAGS) htest3.o hash.c -o htest3

htest3.o: htest3.c
	$(CC) $(CFLAGS) -c htest3.c -o htest3.o

htest4: htest4.o hash.c hash.h
	$(CC) $(CFLAGS) htest4.o hash.c -o htest4

htest4.o: htest4.c
	$(CC) $(CFLAGS) -c htest4.c -o htest4.o

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c -o hash.o

clean:
	rm *.o -i htest1 htest2 htest3 htest4