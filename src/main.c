#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"

int debug = 0;

int main(int argc, char **argv)
{
    int fd, err;
    struct stat st;
    byte *data;
    int64_t sz;

    if (argc != 3) {
        printf("usage: llvmbc-dump <ifile>\n");
        return 0;
    }

    err = stat(argv[1], &st);
    if (err == -1) {
        printf("stat failed with: %s\n", strerror(errno));
        return 0;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("open failed with: %s\n", strerror(errno));
        return 0;
    }

    sz = st.st_size;
    printf("file size: %zd\n", st.st_size);

    data = (byte *) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        printf("mmap failed with: %s\n", strerror(errno));
        return 0;
    }

    if (0) {
        dump(data, 0, st.st_size, 8, (char *) "\t");
    }

    extract(data, st.st_size);

    err = munmap(data, st.st_size);
    if (err != 0) {
        printf("munmap failed with: %s\n", strerror(errno));
        return 0;
    }

    close(fd);
    return 0;
}
