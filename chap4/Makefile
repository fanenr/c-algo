LDFLAGS   = -g
CFLAGS    = -Wall -Wextra -Werror -ggdb3 -std=gnu11
export CFLAGS LDFLAGS MAKEFLAGS

targets := hashmp hashst
objects := $(targets:%=%.o)
export targets objects

.PHONY: all
all: $(objects)

$(objects): %.o: %.c %.h
	gcc -c $< $(CFLAGS)

.PHONY: test
test: $(objects)
	cd tests && make

.PHONY: clean
clean:
	-rm -f *.o
	-cd tests && make clean
