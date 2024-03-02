include config.mk

.PHONY: all
all: bench

bench: bench.o rbmap.o
	gcc $(LDFLAGS) -o $@ $^

bench.o: bench.c rbmap.h
	gcc $(CFLAGS) -c $<

rbmap.o: rbmap.c rbmap.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
