# Makefile for querier
# Builds the querier executable from query.c and its dependencies

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -ggdb
LIBS = -lm

# Executable name
PROG = querier

# Object files
OBJS = query.o

# Build the querier executable
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LIBS)

# Compile query.c
query.o: query.c
	$(CC) $(CFLAGS) -c query.c

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS) *~ core

# Clean everything including test artifacts
.PHONY: distclean
distclean: clean
	rm -rf *.dSYM