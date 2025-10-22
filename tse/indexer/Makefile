CC       = gcc
CFLAGS   = -Wall -pedantic -std=c11 -g \
					 -I.. \
					 -I../utils
LDFLAGS  = -L../lib
LDLIBS   = -lutils -lcurl

indexer: ../lib/libutils.a indexer.o
	$(CC) -o $@ indexer.o $(LDFLAGS) $(LDLIBS)

indexer.o: indexer.c ../webpage.h ../utils/pageio.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f indexer *.o

