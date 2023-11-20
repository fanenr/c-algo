tests := bn_tree

.PHONY: all $(tests)
all: $(tests)

$(tests): %: tests/%

$(tests:%=tests/%): tests/%: tests/%.o %.o
	gcc -g -o $@ $^

$(tests:%=tests/%.o): tests/%.o: tests/%.c %.h
	gcc -g -o $@ -c $<

$(tests:%=%.o): %.o: %.c %.h
	gcc -g -o $@ -c $<


.PHONY: test_all $(tests:%=test_%)
test_all: $(tests:%=test_%)

$(tests:%=test_%): test_%: tests/%
	$<

.PHONY: clean
clean:
	-rm *.o tests/*.o $(tests:%=tests/%)