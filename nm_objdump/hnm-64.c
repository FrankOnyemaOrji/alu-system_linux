#include "hnm.h"

void print_symbol_table64(Elf64_Shdr *section_header, Elf64_Sym *symbol_table,
                          char *string_table, Elf64_Shdr *section_headers)
{
    int symbol_count = section_header->sh_size / sizeof(Elf64_Sym);

    for (int i = 0; i < symbol_count; i++)
    {
        Elf64_Sym current_symbol = symbol_table[i];
        char *symbol_name = string_table + current_symbol.st_name;

        if (current_symbol.st_name == 0 || ELF64_ST_TYPE(current_symbol.st_info) == STT_FILE)
            continue;

        char symbol_type = determine_symbol_type64(&current_symbol, section_headers);

        if (ELF64_ST_BIND(current_symbol.st_info) == STB_LOCAL)
            symbol_type = tolower(symbol_type);

        if (symbol_type != 'U' && symbol_type != 'w')
            printf("%016lx %c %s\n", current_symbol.st_value, symbol_type, symbol_name);
        else
            printf("                 %c %s\n", symbol_type, symbol_name);
    }
}

char determine_symbol_type64(Elf64_Sym *symbol, Elf64_Shdr *section_headers)
{
    if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
        return handle_weak_symbol64(symbol);

    if (symbol->st_shndx == SHN_UNDEF)
        return 'U';
    if (symbol->st_shndx == SHN_ABS)
        return 'A';
    if (symbol->st_shndx == SHN_COMMON)
        return 'C';

    if (symbol->st_shndx < SHN_LORESERVE)
        return handle_regular_symbol64(symbol, &section_headers[symbol->st_shndx]);

    return '?';
}

char handle_weak_symbol64(Elf64_Sym *symbol)
{
    if (symbol->st_shndx == SHN_UNDEF)
        return 'w';
    if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
        return 'V';
    return 'W';
}

char handle_regular_symbol64(Elf64_Sym *symbol, Elf64_Shdr *symbol_section)
{
    if (ELF64_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
        return 'u';

    if (symbol_section->sh_type == SHT_NOBITS &&
        symbol_section->sh_flags == (SHF_ALLOC | SHF_WRITE))
        return 'B';

    if (symbol_section->sh_type == SHT_PROGBITS)
    {
        if (symbol_section->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
            return 'T';
        if (symbol_section->sh_flags == SHF_ALLOC)
            return 'R';
        if (symbol_section->sh_flags == (SHF_ALLOC | SHF_WRITE))
            return 'D';
    }

    if (symbol_section->sh_type == SHT_DYNAMIC)
        return 'D';

    return 't';
}

void process_elf_file64(char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        fprintf(stderr, "./hnm: %s: failed to open file\n", file_path);
        return;
    }

    Elf64_Ehdr elf_header;
    fread(&elf_header, sizeof(Elf64_Ehdr), 1, file);

    if (!validate_elf_file64(&elf_header, file_path))
    {
        fclose(file);
        return;
    }

    Elf64_Shdr *section_headers = read_section_headers64(&elf_header, file);
    if (!section_headers)
    {
        fclose(file);
        return;
    }

    int symbol_table_index = find_symbol_table_index64(&elf_header, section_headers);
    if (symbol_table_index == -1)
    {
        fprintf(stderr, "./hnm: %s: no symbols\n", file_path);
        free(section_headers);
        fclose(file);
        return;
    }

    process_symbol_table64(&elf_header, section_headers, symbol_table_index, file);

    free(section_headers);
    fclose(file);
}

// Additional helper functions would be implemented here