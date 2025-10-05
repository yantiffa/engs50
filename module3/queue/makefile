CFLAGS=-Wall -pedantic -std=c11 -I. -g

list.o: list.c
	gcc $(CFLAGS) -c list.c

testlist.o: testlist.c
	gcc $(CFLAGS) -c testlist.c

testlist: list.o testlist.o
	gcc list.o testlist.o -o testlist

clean:
	rm -f *.o testlist

%.o: %.c %.h
	gcc $(CFLAGS) -c $<
