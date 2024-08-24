CFLAGS=-std=c17 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TARGET=c_compiler

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJS): c_compiler.h

test: $(TARGET)
	./test.sh

clean:
	rm -f $(TARGET) *.o *~ tmp*

.PHONY: test clean
