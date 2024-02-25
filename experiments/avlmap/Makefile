include config.mk

.PHONY: all
all: bench

bench: bench.o avlmap.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c avlmap.h
	gcc $(CFLAGS) -c $<

avlmap.o: avlmap.c avlmap.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
