MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o hashtable.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c hashtable.h
	gcc $(CFLAGS) -c $<

hashtable.o: hashtable.c hashtable.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
