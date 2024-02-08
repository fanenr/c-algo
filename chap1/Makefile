LDFLAGS   = -g
CFLAGS    = -Wall -Wextra -Werror -ggdb3 -std=gnu11

.PHONY: all
all: main
	./main < input.txt

main: main.o
	gcc -o $@ $^ $(LDFLAGS)

main.o: main.c
	gcc -c $< $(CFLAGS)

clean:
	-rm -f *.o main
