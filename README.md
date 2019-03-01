# llvmbc dump

Simple tool that extracts LLVM bitcode modules from ELF binaries compiled with "-fembed-bitcode".

## Compile

mkdir build
cd build
cmake ..
make

## Usage

> src/llvmbc-dump tests/src/test

```text
[xigh@bsd64 ~/src/llvmbc-dump/build]$ ./src/llvmbc-dump tests/src/test 
file size: 32144
module: /home/xigh/src/llvmbc-dump/tests/src/main.c / x86_64-unknown-freebsd12.0 / e-m:e-i64:64-f80:128-n8:16:32:64-S128
	F: main: i32 (i32, i8**)*
	D: foo: i32 (...)*
	D: bar: i32 (...)*
module: /home/xigh/src/llvmbc-dump/tests/lib/foo.c / x86_64-unknown-freebsd12.0 / e-m:e-i64:64-f80:128-n8:16:32:64-S128
	F: foo: i32 ()*
module: /home/xigh/src/llvmbc-dump/tests/lib/bar.c / x86_64-unknown-freebsd12.0 / e-m:e-i64:64-f80:128-n8:16:32:64-S128
	F: bar: i32 ()*
```

## TODO

* Test it on bigger projects
* finish analyzeFn

## Copyright

BSD
