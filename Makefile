librssgen.so: rssgen.o
	ld -shared -o librssgen.so rssgen.o
install: librssgen.so
	install librssgen.so /usr/lib
	install rssgen.h /usr/include
rssgen.o: rssgen.c
	gcc -c -O3 rssgen.c
clean:
	rm librssgen.so rssgen.o

.PHONY: install clean
