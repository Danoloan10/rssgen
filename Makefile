test: test.c
	gcc test.c libncunit.a -o test
	./test
