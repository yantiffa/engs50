CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -I../utils -I../lib -I.. -g
LIB = ../lib/libutils.a
<<<<<<< HEAD
TARGET := pageiotest pageio

all: $(TARGET)
$(OBJS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIB) -lcurl -o $(TARGET)
=======
LDFLAGS = -lcurl

SRCS := $(wildcard *.c)
TEST_SRCS := $(filter test_%.c,$(SRCS))
MAIN_SRCS := $(filter pageiotest.c,$(SRCS))
OTHER_SRCS := $(filter-out $(TEST_SRCS) $(MAIN_SRCS),$(SRCS))

OBJS_OTHER := $(OTHER_SRCS:.c=.o)
OBJS_MAIN := $(MAIN_SRCS:.c=.o)
TEST_TARGETS := $(TEST_SRCS:.c=)

.PHONY: all clean
all: pageiotest $(TEST_TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the primary executable that provides the standalone main
pageiotest: $(OBJS_MAIN) $(OBJS_OTHER)
	$(CC) $(OBJS_MAIN) $(OBJS_OTHER) $(LIB) $(LDFLAGS) -o $@

# Build each test executable without linking the main program object(s)
# This prevents multiple-definition-of-main when a pageiotest.c exists.
%: $(OBJS_OTHER) %.o
	$(CC) $^ $(LIB) $(LDFLAGS) -o $@

>>>>>>> c39944313ef2942b4697c11dc2667be0c32eaaec
clean:
	rm -f *.o pageiotest $(TEST_TARGETS)