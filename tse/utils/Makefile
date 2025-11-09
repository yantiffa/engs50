CFLAGS=-Wall -pedantic -std=c11 -I. -I.. -I../utils/module/module3/queue -g
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
