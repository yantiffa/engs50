CFLAGS=-Wall -pedantic -std=c11 -I. -g
	
OFILES = queue.o hash.o webpage.o indexio.o pageio.o
LIBDIR = lib
LIBNAME = libutils.a

all: $(LIBDIR)/$(LIBNAME)

indexio.o: utils/indexio.c
	gcc $(CFLAGS) -c $< -o $@

pageio.o: utils/pageio.c
	gcc $(CFLAGS) -c $< -o $@
	
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(LIBDIR)/$(LIBNAME): $(OFILES) | $(LIBDIR)
	ar cr $@ $(OFILES)

$(LIBDIR):
	mkdir -p $(LIBDIR)

clean:
	rm -f *.o $(LIBDIR)/$(LIBNAME)
