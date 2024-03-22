MODE = debug

include config.mk
export CFLAGS LDFLAGS

targets := vector list hashmap avltree rbmap
objects := $(targets:%=%.o)
export targets objects

.PHONY: all
all: $(objects)

$(objects): %.o: %.c %.h
	gcc $(CFLAGS) -c $< 

.PHONY: test run
test: $(objects)
	cd tests && make

run: $(objects)
	cd tests && make run

.PHONY: json
json:
	make clean && bear -- make test

.PHONY: clean
clean:
	-rm -f *.o && cd tests && make clean
