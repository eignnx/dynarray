build: dynarray.o

test: tests/tests.c dynarray.o
	cp dynarray.o tests/
	cp dynarray.h tests/
	clang -g $^ -o out/tests
	out/tests

dynarray.o: dynarray.c dynarray.h
	clang -g -c dynarray.c -o dynarray.o

clean:
	rm -fv *.o
	rm -rfv out/**
	rm -fv tests/dynarray.*
