CFLAGS = -Wall -pedantic -std=c11 -I../utils -I../lib -I.. -g
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
LIB = ../lib/libutils.a
TARGET := pageiotest
all: $(TARGET)
$(OBJS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIB) -lcurl -o $(TARGET)
clean:
	rm -f *.o $(TARGET)
