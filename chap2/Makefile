tests := slist dlist stack queue

.PHONY: all $(tests)
all: $(tests)

$(tests): %: tests/%

$(tests:%=tests/%): tests/%: tests/%.o %.o
	gcc -g -o $@ $^

$(tests:%=tests/%.o): tests/%.o: tests/%.c %.h
	gcc -g -o $@ -c $<

$(tests:%=%.o): %.o: %.c %.h
	gcc -g -o $@ -c $<

.PHONY: clean
clean:
	-rm *.o tests/*.o $(tests:%=tests/%)