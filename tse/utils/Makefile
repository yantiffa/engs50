CFLAGS=-Wall -pedantic -std=c11 -I. -I.. -I./module/module3/queue -I./module/module3/hash -g
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
LIB = ../lib/libutils.a

all: $(LIB)
$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

clean:
	rm -f *.o

.PHONY:
	clean all
