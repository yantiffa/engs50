CC       = gcc
CFLAGS   = -Wall -pedantic -std=c11 -g \
		   -I.. \
		   -I../utils/module/module3/queue \
		   -I../utils/module/module3/hash
LDFLAGS  = -L../lib
LDLIBS   = -lutils -lcurl
PAGEDIR  := /workspaces/modules/tse/pagedir

crawler: ../lib/libutils.a crawler.o
	$(CC) -o $@ crawler.o $(LDFLAGS) $(LDLIBS)

crawler.o: crawler.c ../webpage.h \
					 ../utils/module/module3/queue/queue.h \
					 ../utils/module/module3/hash/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

tcrawler: ../lib/libutils.a tcrawler.o
	$(CC) -o $@ tcrawler.o $(LDFLAGS) $(LDLIBS)

tcrawler.o: tcrawler.c ../webpage.h \
					   ../utils/module/module3/queue/queue.h \
					   ../utils/module/module3/hash/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

counts: crawler $(PAGEDIR)
	@rm -f $(PAGEDIR)/*
	@for d in 0 1 2 3; do \
	  ./crawler "https://thayer.github.io/engs50/" $(PAGEDIR) $$d >/dev/null || true; \
	  n=$$(ls -1 pagedir/[0-9]* 2>/dev/null | wc -l); \
	  echo "$$d: $$n"; \
	  rm -f $(PAGEDIR)/*; \
	done

	@echo "" >> crawl_counts.txt
	@echo "(3 Points) TA Signature:" >> crawl_counts.txt
	@echo "Wrote crawl_counts.txt"


clean:
	rm -f crawler tcrawler *.o