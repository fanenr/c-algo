targets := slist dlist stack queue deque
objects := $(targets:%=%.o)
tests := $(targets:%=test_%)

.PHONY: all $(tests)
all: $(tests)

$(tests): test_%: %
	./$<

$(targets): %: %.o ../%.o
	gcc -g -o $@ $^

$(objects): %.o: %.c ../%.h
	gcc -g -c $<

$(objects:%=../%): ../%.o: ../%.c ../%.h
	cd .. && make $(subst ../,,$@)