#include <stdio.h>
#include <elf.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

byte head[] = { 0x42, 0x43, 0xc0, 0xde };

void extract(byte* ptr, int64_t len) {
    Elf64_Ehdr *h;
	Elf64_Shdr *s;
	byte *bc;
	size_t sz;
	int i, fd;
	int64_t w;
	byte c;
	char *n;

    if (len < (int64_t) sizeof(Elf64_Ehdr)) {
        printf("file too small\n");
        return;
    }
    
    h = (Elf64_Ehdr *) ptr;
	if (h->e_ident[EI_MAG0] != 0x7f &&
		h->e_ident[EI_MAG1] != 'E' &&
		h->e_ident[EI_MAG2] != 'L' &&
		h->e_ident[EI_MAG3] != 'F') {
		printf("not ELF file\n");
		return;
	}

	c = h->e_ident[EI_CLASS];
	if (c != ELFCLASS64) {
		printf("32 bits ELF not supported [%d]", c);
		return;
	}

	if (h->e_ident[EI_DATA] != ELFDATA2LSB) {
		printf("unsupported ELF File byte order\n");
		return;
	}

	if (h->e_ident[EI_VERSION] != EV_CURRENT) {
		printf("unsupported ELF File version\n");
		return;
	}

	if (h->e_type != ET_REL && h->e_type != ET_EXEC) {
		printf("unsupported ELF File type\n");
		return;
	}

	if (h->e_shstrndx == SHN_UNDEF) {
		printf("ELF file without string table\n");
		return;
	}

	s = (Elf64_Shdr *) (ptr + h->e_shoff) + h->e_shstrndx;
	n = (char *) ptr + s->sh_offset;

	if (debug) {
		printf("e_type= %d\n", h->e_type);          	/* File type. */
		printf("e_machine= %d\n", h->e_machine);        /* Machine architecture. */
		printf("e_version= %d\n", h->e_version);        /* ELF format version. */
		printf("e_entry= %lu\n", h->e_entry);			/* Entry point. */
		printf("e_phoff= %lu\n", h->e_phoff);			/* Program header file offset. */
		printf("e_shoff= %lu\n", h->e_shoff);			/* Section header file offset. */
		printf("e_flags= %d\n", h->e_flags);            /* Architecture-specific flags. */
		printf("e_ehsize= %d\n", h->e_ehsize);          /* Size of ELF header in bytes. */
		printf("e_phentsize= %d\n", h->e_phentsize);    /* Size of program header entry. */
		printf("e_phnum= %d\n", h->e_phnum);            /* Number of program header entries. */
		printf("e_shentsize= %d\n", h->e_shentsize);    /* Size of section header entry. */
		printf("e_shnum= %d\n", h->e_shnum);            /* Number of section header entries. */
		printf("e_shstrndx= %d\n", h->e_shstrndx);      /* Section name strings section. */
	}

	for (i = 0; i < h->e_shnum; i += 1) {
		s = (Elf64_Shdr *) (ptr + h->e_shoff) + i;
		if (strcmp(n + s->sh_name, ".llvmbc")) {
			continue;
		}

		if (debug) {
			printf("%-3d %s\n", i, n + s->sh_name);
			printf("   sh_name = %u\n", s->sh_name);
			printf("   sh_type = %u\n", s->sh_type);
			printf("   sh_flags = %lu\n", s->sh_flags);
			printf("   sh_addr = %lu\n", s->sh_addr);
			printf("   sh_offset = %lu\n", s->sh_offset);
			printf("   sh_size = %lu\n", s->sh_size);
			printf("   sh_link = %u\n", s->sh_link);
			printf("   sh_info = %u\n", s->sh_info);
			printf("   sh_addralign = %lu\n", s->sh_addralign);
			printf("   sh_entsize = %lu\n", s->sh_entsize);
		}

		bc = ptr + s->sh_offset;
		sz = s->sh_size;

		while (sz > 0) {
			// skip 0 padded bytes

			while (sz > 0) {
				if (bc[0] != 0) {
					break;
				}
				sz -= 1;
				bc += 1;
			}

			// get module size, if any

			uint32_t n = dumpbits(bc, sz);
			if (n == 0) {
				break;
			}

			// dump module

			readmod(bc, n);
			
			// next module

			bc += n;
			sz -= n;
		}
	}
}
