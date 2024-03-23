MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o avltree.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c avltree.h
	gcc $(CFLAGS) -c $<

avltree.o: avltree.c avltree.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
