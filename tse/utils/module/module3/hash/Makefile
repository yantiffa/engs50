CFLAGS=-Wall -pedantic -std=c11 -I. -g

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TARGET := test_hash

all: $(TARGET)

$(OBJS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	gcc $(OBJS) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)
