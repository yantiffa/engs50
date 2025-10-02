all: test1 test2

test1.o: test1.c
	gcc -c test1.c

test2.o: test2.c
	gcc -c test2.c

integrate.o: integrate.c
	gcc -c integrate.c

test1: test1.o integrate.o
	gcc integrate.o test1.o -o test1 -lm

test2: test2.o integrate.o
	gcc integrate.o test2.o -o test2 -lm

clean:
	rm -f *.o test1 test2
