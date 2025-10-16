CC       = gcc
CFLAGS   = -Wall -pedantic -std=c11 -g \
           -I.. \
           -I../utils/module/module3/queue \
           -I../utils/module/module3/hash
LDFLAGS  = -L../lib
LDLIBS   = -lutils -lcurl

crawler: ../lib/libutils.a crawler.o
	$(CC) -o $@ crawler.o $(LDFLAGS) $(LDLIBS)

crawler.o: crawler.c ../webpage.h \
                     ../utils/module/module3/queue/queue.h \
                     ../utils/module/module3/hash/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f crawler *.o