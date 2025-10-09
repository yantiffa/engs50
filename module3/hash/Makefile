CFLAGS=-Wall -pedantic -std=c11 -I. -g

hash.o: hash.c
	gcc $(CFLAGS) -c hash.c

test_hash.o: test_hash.c
	gcc $(CFLAGS) -c test_hash.c

test_hash: hash.o test_hash.o
	gcc hash.o test_hash.o -o test_hash

clean:
	rm -f *.o test_hash


