OBJS=dump.o extr.o bit.o mod.o main.o
LLVM_CONFIG=llvm-config-devel
CFLAGS=-fembed-bitcode `${LLVM_CONFIG} --cflags`
LDFLAGS=-fembed-bitcode `${LLVM_CONFIG} --cxxflags --ldflags --libs core bitreader --system-libs`
CC=clang
LD=clang++
BIN=llvmbc-dump

all: $(BIN)

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

bit.o: bit.c defs.h
	$(CC) -o $@ -c $< $(CFLAGS)

mod.o: mod.c defs.h
	$(CC) -o $@ -c $< $(CFLAGS)

dump.o: dump.c defs.h
	$(CC) -o $@ -c $< $(CFLAGS)

extr.o: extr.c defs.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
main.o: main.c defs.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean: .PHONY
	rm -f *.o *.core $(BIN)
