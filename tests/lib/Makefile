OBJS=foo.o bar.o
CFLAGS=-fembed-bitcode
CC=clang
AR=ar

all: libfoo.a

libfoo.a: $(OBJS)
	$(AR) cr $@ $(OBJS)

foo.o: foo.c
	$(CC) -o $@ -c $< $(CFLAGS)

bar.o: bar.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean: .PHONY
	rm -f *.o *.core *.a
