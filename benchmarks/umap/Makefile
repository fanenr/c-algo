PG   =
MODE = release

include config.mk

.PHONY: all
all: bench

bench: bench.o
	g++ $(PG) $(LDFLAGS) -o $@ $^

bench.o: bench.cc
	g++ $(PG) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f *.o bench

.PHONY: json
json: clean
	bear -- make
