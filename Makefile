install: librssgen.so
	install librssgen.so /usr/lib
	install rssgen.h /usr/include
rssgen.o:
	gcc -c -O3 rssgen.c
librssgen.so: rssgen.o
	ld -shared -o librssgen.so rssgen.o

.PHONY: install
