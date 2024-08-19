CFRAGS=-std=c17 -g -static

sample: sample.c

.PHONY: test
test: sample
	./sample-test.sh
	rm -f sample *.o *~ tmp*
