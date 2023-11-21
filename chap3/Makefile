targets := bntree
objects := $(targets:%=%.o)

.PHONY: all
all: $(objects)

$(objects): %.o: %.c %.h
	gcc -g -c $<

.PHONY: test
test:
	cd tests && make

.PHONY: clean
clean:
	-rm *.o **/*.o $(targets:%=tests/%)