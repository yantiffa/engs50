CC       = gcc
CFLAGS   = -Wall -pedantic -std=c11 -g \
           -I.. \
           -I../utils \
           -I../utils/module/module3/hash
LDFLAGS  = -L../lib
LDLIBS   = -lutils -lcurl

indexer: indexer.o ../utils/pageio.o
	$(CC) -o $@ indexer.o ../utils/pageio.o $(LDFLAGS) $(LDLIBS)

indexer.o: indexer.c ../webpage.h ../utils/pageio.h ../utils/module/module3/hash/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

../utils/pageio.o: ../utils/pageio.c ../webpage.h ../utils/pageio.h
	$(CC) $(CFLAGS) -c ../utils/pageio.c -o ../utils/pageio.o

clean:
	rm -f indexer *.o

