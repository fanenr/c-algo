tasks := task_test_expr

.PHONY: all
all: $(tasks)

task_test_expr: test_expr
	./test_expr < input.txt
	@echo "$< task finished"

test_expr: test_expr.o expr.o ../slist.o ../stack.o
	gcc -o $@ $^ $(LDFLAGS)

test_expr.o: test_expr.c expr.h
	gcc -c $< $(CFLAGS)

expr.o: expr.c expr.h
	gcc -c $< $(CFLAGS)

../slist.o ../stack.o: ../%.o: ../%.c ../%.h
	cd .. && make $(subst ../,,$@)

clean:
	-rm -f *.o test_expr
