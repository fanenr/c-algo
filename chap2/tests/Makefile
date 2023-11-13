all: slist

slist: ../slist.o slist.o
	gcc -o slist $^

slist.o: slist.c ../slist.h
	gcc -g -c $<