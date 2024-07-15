#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <fcntl.h>
#include <gelf.h>
#include <unistd.h>

void print_section_contents(const char *filename) {
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
        printf("Contents of section %s:\n", elf_strptr(e, shstrndx, shdr.sh_name));
        Elf_Data *data = elf_getdata(scn, NULL);
        for (size_t i = 0; i < data->d_size; i++) {
            printf("%02x ", ((unsigned char *)data->d_buf)[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
        if (data->d_size % 16 != 0)
            printf("\n");
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
        print_section_contents(argv[i]);
    }

    return 0;
}
