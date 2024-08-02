#include "hnm.h"

char get_weak_symbol_type(Elf32_Sym symbol)
{
	if (symbol.st_shndx == SHN_UNDEF)
		return ('w');
	return ((ELF32_ST_TYPE(symbol.st_info) == STT_OBJECT) ? 'V' : 'W');
}

char get_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	char symbol_type = '?';

	if (ELF32_ST_BIND(symbol.st_info) == STB_WEAK)
		symbol_type = get_weak_symbol_type(symbol);
	else if (symbol.st_shndx == SHN_UNDEF)
		symbol_type = 'U';
	else if (symbol.st_shndx == SHN_ABS)
		symbol_type = 'A';
	else if (symbol.st_shndx == SHN_COMMON)
		symbol_type = 'C';
	else if (symbol.st_shndx < SHN_LORESERVE)
		symbol_type = get_section_symbol_type(symbol, section_headers);

	return ((ELF32_ST_BIND(symbol.st_info) == STB_LOCAL) ?
		tolower(symbol_type) : symbol_type);
}

char get_section_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	Elf32_Shdr symbol_section = section_headers[symbol.st_shndx];

	if (ELF32_ST_BIND(symbol.st_info) == STB_GNU_UNIQUE)
		return ('u');
	if (symbol_section.sh_type == SHT_NOBITS &&
	    symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ('B');
	if (symbol_section.sh_type == SHT_PROGBITS)
	{
		if (symbol_section.sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
			return ('T');
		if (symbol_section.sh_flags == SHF_ALLOC)
			return ('R');
		if (symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
			return ('D');
	}
	if (symbol_section.sh_type == SHT_DYNAMIC)
		return ('D');
	return ('t');
}

void print_symbol(Elf32_Sym symbol, char *symbol_name, char symbol_type)
{
	if (symbol_type != 'U' && symbol_type != 'w')
		printf("%08x %c %s\n", symbol.st_value, symbol_type, symbol_name);
	else
		printf("         %c %s\n", symbol_type, symbol_name);
}

void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
			  char *string_table, Elf32_Shdr *section_headers)
{
	int i, symbol_count = section_header->sh_size / sizeof(Elf32_Sym);
	char *symbol_name, symbol_type;

	for (i = 0; i < symbol_count; i++)
	{
		Elf32_Sym symbol = symbol_table[i];
		symbol_name = string_table + symbol.st_name;


		if (symbol.st_name != 0 && ELF32_ST_TYPE(symbol.st_info) != STT_FILE)
		{
			symbol_type = get_symbol_type(symbol, section_headers);
			print_symbol(symbol, symbol_name, symbol_type);
		}
	}
}

void process_elf_file32(char *file_path);
int validate_elf_file(Elf32_Ehdr *elf_header, char *file_path);
int find_symbol_table(Elf32_Shdr *section_headers, int shnum);
void cleanup(FILE *file, void *section_headers, void *symbol_table,
	     void *string_table);

