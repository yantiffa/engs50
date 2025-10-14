CFLAGS=-Wall -pedantic -std=c11 -I. -g

test_queue: queue.o test_queue.o
	gcc queue.o test_queue.o -o test_queue

clean:
	rm -f *.o test_queue

%.o: %.c %.h
	gcc $(CFLAGS) -c $<
