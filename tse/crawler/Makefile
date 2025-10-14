CFLAGS=-Wall -pedantic -std=c11 -I. -L$(LIBDIR) -g
LIBDIR = lib
LIBNAME = libutils.a
LIBS=-lutils -lcurl

all: $(LIBDIR)/$(LIBNAME) crawler

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(LIBDIR)/$(LIBNAME): $(OFILES) | $(LIBDIR)
	ar cr $@ $(OFILES)

$(LIBDIR):
	mkdir -p $(LIBDIR)

crawler: crawler.c $(LIBDIR)/$(LIBNAME)
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

clean:
	rm -f *.o $(LIBDIR)/$(LIBNAME) crawler
