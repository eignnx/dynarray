build: tests.c dynarray.o
	clang -g $^ -o out/tests

test: build
	out/tests

dynarray.o: dynarray.c dynarray.h
	clang -c dynarray.c -o dynarray.o

clean:
	rm -fv *.o
	rm -rfv out/**
