MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o rbtree.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c rbtree.h
	gcc $(CFLAGS) -c $<

rbtree.o: rbtree.c rbtree.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
