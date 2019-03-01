OBJS=main.o
CFLAGS=-fembed-bitcode
LDFLAGS=-fembed-bitcode
LIBS=../lib/libfoo.a
CC=clang
LD=clang++

all: test

test: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

main.o: main.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean: .PHONY
	rm -f *.o *.core
