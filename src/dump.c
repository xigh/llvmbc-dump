#include <stdio.h>
#include <ctype.h>
#include "defs.h"

void dump(byte* ptr, int64_t off, int64_t len, int lines, char *indent) {
    int64_t i, j;
    byte c;

	for (i = 0; i < len-off; i++) {
		if (i%16 == 0) {
			if (i > 0) {
				printf("   ");
				for (j = i - 16; j < i; j++) {
					c = ptr[off+j];
					if (!isprint(c)) {
						c = '-';
					}
					printf("%c ", c);
				}
				printf("\n");
				if (i/16 >= lines) {
					return;
				}
			}
			printf("%s%016lx: ", indent, off+i);
		}
		printf("%02x ", ptr[off+i]);
	}
    j = i;
    while (j%16 != 0) {
        printf("-- ");
        j++;
    }
    printf("   ");
    for (j = i & ~0xf; j < len-off; j++) {
        c = ptr[off+j];
        if (!isprint(c)) {
            c = '-';
        }
        printf("%c ", c);
    }
    printf("\n");
}
