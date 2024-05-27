PG   =
MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o hashtable.o
	gcc $(PG) $(LDFLAGS) -o $@ $^

bench.o: bench.c hashtable.h
	gcc $(PG) $(CFLAGS) -c $<

hashtable.o: hashtable.c hashtable.h
	gcc $(PG) $(CFLAGS) -c $<

.PHONY: json
json: clean
	bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
