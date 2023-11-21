targets := bntree
objects := $(targets:%=%.o)
tests := $(targets:%=test_%)

.PHONY: all $(tests)
all: $(tests)

$(tests): test_%: %
	./$<
	@echo "$< test passed"

$(targets): %: %.o ../%.o
	gcc -g -o $@ $^

$(objects): %.o: %.c ../%.h
	gcc -g -c $<

$(objects:%=../%): ../%.o: ../%.c ../%.h
	cd .. && make $(subst ../,,$@)