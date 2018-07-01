CC      = g++
CFLAGS  = -c -o3 -Wall
LDFLAGS =

all: hello test

hello: hello.o
	$(CC) $< -o $@
hello.o: hello.cpp
	$(CC) $(CFLAGS) $< -o $@
test: test.o
	$(CC) $(LDFLAGS) $< -o $@
test.o: test.cpp
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *.o hello test
