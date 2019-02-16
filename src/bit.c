#include <llvm-c/Core.h>
#include <llvm-c/BitReader.h>
#include <stdio.h>
#include <assert.h>

#include "defs.h"

struct stream {
    byte *buf;
    int64_t sz;
    uint32_t w;
    uint32_t b;
};

void fillword(struct stream *s) {
    byte *buf = s->buf;
    s->w = (buf[0] << 0) | (buf[1] << 8) |
        (buf[2] << 16) | (buf[3] << 24);
    s->b = 32;
    s->sz -= 4;
    s->buf += 4;
}

void skipword(struct stream *s) {
    s->b = 0;
}

void eatbytes(struct stream *s, uint32_t n) {
    assert(s->b == 0);
    s->sz -= n;
    s->buf += n;
}

uint32_t readbits(struct stream *s, uint32_t n) {
    uint32_t r, r2, l;

    if (n > 32) {
        printf("readbits, unexpected %d\n", n);
    }

    assert(n >= 0 && n <= 32);

    if (s->b >= n) {
        r = s->w;
        r &= 0xffffffff >> (32 - n);
        s->w >>= (n & 0x1f);
        s->b -= n;
        return r;
    }

    r = (s->b > 0) ? s->w : 0;
    l = n - s->b;

    fillword(s);

    assert(l <= s->b);

    r2 = s->w;
    r2 &= 0xffffffff >> (32 - l);
    s->w >>= (l & 0x1f);
    s->b -= l;

    return r | (r2 << (n - l));
}

uint32_t readvbr(struct stream *s, uint32_t n) {
    uint32_t p, r, b;

    p = readbits(s, n);
    if ((p & (1 << (n - 1))) == 0)
        return p;
    
    r = b = 0;
    for (;;) {
        r |= (p & ((1U << (n-1))-1)) << n;
        
        if ((p & (1U << (n-1))) == 0)
            break;
            
        n += n-1;
        p = readbits(s, n);
    }

    return r;
}

uint32_t dumpbits(byte *buf, int64_t sz) {
    struct stream s = {
        .buf= buf,
        .sz= sz,
        .w= 0,
        .b= 0,
    };
    uint8_t sig[6];
    uint32_t v, n, p, x;

    // Is this IR Bitstream

    sig[0] = readbits(&s, 8);
    sig[1] = readbits(&s, 8);

    if (sig[0] != 'B' || sig[1] != 'C') {
        // printf("# not Bitcode: %x %x\n", sig[0], sig[1]);
        return 0;
    }

    sig[2] = readbits(&s, 4);
    sig[3] = readbits(&s, 4);
    sig[4] = readbits(&s, 4);
    sig[5] = readbits(&s, 4);

    if (sig[2] != 0x0 || sig[3] != 0xC ||
        sig[4] != 0xE || sig[5] != 0xD) {
        printf("# not Bitstream: %x %x %x %x\n",
            sig[2], sig[3], sig[4], sig[5]);
        return 0;
    }

    if (debug) {
        printf("# found IR Bitstream (sz=%ld)\n", sz);
    }

    x = 4;
    for (;;) {
        p = s.buf - buf;

        if (p > 4 && s.buf[0] == 'B' && s.buf[1] == 'C' && s.buf[2] == 0xc0 && s.buf[3] == 0xde) {
            break;
        }

        v = readvbr(&s, 4);
        if (debug) {
            printf("#\tvbr=%d\n", v);
        }

        skipword(&s);
        if (v == 0) {
            break;
        }

        // block size, in words
        n = 4 * readbits(&s, 32);
        if (n == 0) {
            break;
        }
        
        if (debug) {
            printf("#\tblock %08x: %d bytes\n", p, n);
        }

        if (n > s.sz) {
            if (debug) {
                printf("#\tn=%d, s.sz=%ld\n", n, s.sz);
            }
            break;
        }

        x += n;
        eatbytes(&s, n);
    }

    p = s.buf - buf;

    if (debug) {
        printf("# size=%d / %d\n", p, x);
    }

    return p;
}
