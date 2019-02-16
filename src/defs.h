#include <stdint.h>

typedef unsigned char byte;
extern void readmod(byte* ptr, int64_t len);
extern void extract(byte* ptr, int64_t len);
extern void dump(byte* ptr, int64_t off, int64_t len, int lines, char *indent);

extern uint32_t dumpbits(byte *buf, int64_t sz);

extern int debug;
