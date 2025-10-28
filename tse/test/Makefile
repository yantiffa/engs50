CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -I../utils -I../lib -I.. -g
LIB = ../lib/libutils.a
LDFLAGS = -lcurl

SRCS := $(wildcard *.c)
TEST_SRCS := $(filter test_%.c,$(SRCS))
MAIN_SRCS := $(filter pageiotest.c,$(SRCS))
OTHER_SRCS := $(filter-out $(TEST_SRCS) $(MAIN_SRCS),$(SRCS))

OBJS_OTHER := $(OTHER_SRCS:.c=.o) ../utils/indexio.o
TEST_TARGETS := $(TEST_SRCS:.c=)

.PHONY: all clean
all: pageiotest $(TEST_TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build pageiotest as standalone executable
pageiotest: pageiotest.o $(OBJS_OTHER)
	$(CC) $^ $(LIB) $(LDFLAGS) -o $@

# Build each test executable as standalone (each has its own main)
test_%: test_%.o $(OBJS_OTHER)
	$(CC) $^ $(LIB) $(LDFLAGS) -o $@

clean:
	rm -f *.o pageiotest $(TEST_TARGETS)
