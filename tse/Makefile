CFLAGS=-Wall -pedantic -std=c11 -I. -g

pageio.o: utils/pageio.c
	gcc $(CFLAGS) -c $< -o $@
OFILES = queue.o hash.o webpage.o pageio.o
LIBDIR = lib
LIBNAME = libutils.a

all: $(LIBDIR)/$(LIBNAME)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(LIBDIR)/$(LIBNAME): $(OFILES) | $(LIBDIR)
	ar cr $@ $(OFILES)

$(LIBDIR):
	mkdir -p $(LIBDIR)

clean:
	rm -f *.o $(LIBDIR)/$(LIBNAME)
