mod_blocklist.la: mod_blocklist.c
	apxs -c mod_blocklist.c -lblocklist

clean:
	rm -rf .libs
	rm -f mod_blocklist.la mod_blocklist.lo mod_blocklist.o mod_blocklist.slo

install: mod_blocklist.la
	apxs -i -a mod_blocklist.la

.PHONY: clean install
