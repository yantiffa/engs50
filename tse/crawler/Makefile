CFLAGS= -Wall -pedantic -std=c11 -I../ -L../lib/ -g
LIBS=-lutils -lcurl

crawler:
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

clean:
	rm -f crawler *.o
