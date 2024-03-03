MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o hashmap.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c hashmap.h
	gcc $(CFLAGS) -c $<

hashmap.o: hashmap.c hashmap.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
