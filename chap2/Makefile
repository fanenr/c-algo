LDFLAGS   = -g
MAKEFLAGS = --no-print-directory
CFLAGS    = -Wall -Wextra -Werror -ggdb -std=gnu11
export CFLAGS LDFLAGS MAKEFLAGS

targets := vector slist dlist stack queue deque
objects := $(targets:%=%.o)
sources := $(shell find . -name "*.c")
export targets objects

.PHONY: all
all: $(objects)

$(objects): %.o: %.c %.h
	gcc -c $< $(CFLAGS)

.PHONY: test
test: $(objects)
	cd tests && make

.PHONY: sample
sample: slist.o stack.o
	cd samples && make

.PHONY: clean
clean:
	-rm -f *.o
	-cd tests && make clean
	-cd samples && make clean