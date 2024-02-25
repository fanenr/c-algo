include config.mk

.PHONY: all
all: bench

bench: bench.o
	g++ $(LDFLAGS) -o $@ $^

bench.o: bench.cc
	g++ $(CFLAGS) -c $<

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o bench
