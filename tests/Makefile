tests := $(targets:%=test_%)

.PHONY: all $(tests)
all: $(tests)

$(tests): test_%: %
	./$<
	@echo "$< test passed"

$(targets): %: %.o ../%.o
	gcc $(LDFLAGS) -o $@ $^

$(objects): %.o: %.c ../%.h
	gcc $(CFLAGS) -c $< 

$(objects:%=../%): ../%.o: ../%.c ../%.h
	cd .. && make $(subst ../,,$@)

clean:
	-rm -f *.o $(targets)
