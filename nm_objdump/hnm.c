#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <fcntl.h>
#include <gelf.h>
#include <unistd.h>

void print_symbols(const char *filename) {
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed: %s\n", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    Elf *e = elf_begin(fd, ELF_C_READ, NULL);
    if (e == NULL) {
        fprintf(stderr, "elf_begin() failed: %s.\n", elf_errmsg(-1));
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (elf_kind(e) != ELF_K_ELF) {
        fprintf(stderr, "%s is not an ELF object.\n", filename);
        elf_end(e);
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t shstrndx;
    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        fprintf(stderr, "elf_getshdrstrndx() failed: %s.\n", elf_errmsg(-1));
        elf_end(e);
        close(fd);
        exit(EXIT_FAILURE);
    }

    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    while ((scn = elf_nextscn(e, scn)) != NULL) {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM) {
            Elf_Data *data = elf_getdata(scn, NULL);
            int count = shdr.sh_size / shdr.sh_entsize;
            for (int i = 0; i < count; ++i) {
                GElf_Sym sym;
                gelf_getsym(data, i, &sym);
                printf("%016lx %c %s\n", sym.st_value,
                       GELF_ST_TYPE(sym.st_info) == STT_FUNC ? 'T' : 'U',
                       elf_strptr(e, shdr.sh_link, sym.st_name));
            }
        }
    }

    elf_end(e);
    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        print_symbols(argv[i]);
    }

    return 0;
}
