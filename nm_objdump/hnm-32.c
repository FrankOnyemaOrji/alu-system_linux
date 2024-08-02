#include "hnm.h"

/**
 * get_symbol_type - Get the symbol type based on various conditions.
 * @symbol: Pointer to the symbol.
 * @symbol_section: Section header of the symbol.
 *
 * Return: Character representing the symbol type.
 */
char get_symbol_type(Elf32_Sym *symbol, Elf32_Shdr *symbol_section)
{
	char symbol_type = '?';

	if (ELF32_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		if (symbol->st_shndx == SHN_UNDEF)
			symbol_type = 'w';
		else if (ELF32_ST_TYPE(symbol->st_info) == STT_OBJECT)
			symbol_type = 'V';
		else
			symbol_type = 'W';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		symbol_type = 'U';
	else if (symbol->st_shndx == SHN_ABS)
		symbol_type = 'A';
	else if (symbol->st_shndx == SHN_COMMON)
		symbol_type = 'C';
	else if (ELF32_ST_BIND(symbol->st_info) == STB_GNU_UNIQUE)
		symbol_type = 'u';
	else if (symbol_section->sh_type == SHT_NOBITS &&
		symbol_section->sh_flags == (SHF_ALLOC | SHF_WRITE))
		symbol_type = 'B';
	else if (symbol_section->sh_type == SHT_PROGBITS)
	{
		if (symbol_section->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
			symbol_type = 'T';
		else if (symbol_section->sh_flags == SHF_ALLOC)
			symbol_type = 'R';
		else if (symbol_section->sh_flags == (SHF_ALLOC | SHF_WRITE))
			symbol_type = 'D';
	}
	else if (symbol_section->sh_type == SHT_DYNAMIC)
		symbol_type = 'D';
	else
		symbol_type = 't';

	return (symbol_type);
}

/**
 * print_symbol_type - Determine the symbol type.
 * @symbol: Pointer to the symbol.
 * @symbol_type: Pointer to the character to store the symbol type.
 * @section_headers: Array of section headers.
 */
void print_symbol_type(Elf32_Sym *symbol,
char *symbol_type, Elf32_Shdr *section_headers)
{
	Elf32_Shdr symbol_section = section_headers[symbol->st_shndx];
	*symbol_type = '?';

	if (symbol->st_name != 0 && ELF32_ST_TYPE(symbol->st_info) != STT_FILE)
	{
		*symbol_type = get_symbol_type(symbol, &symbol_section);

		if (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
			*symbol_type = tolower(*symbol_type);
	}
}

/**
 * print_symbol32 - Print the symbol information.
 * @symbol: Pointer to the symbol.
 * @string_table: Pointer to the string table.
 * @section_headers: Array of section headers.
 */
void print_symbol32(Elf32_Sym *symbol,
char *string_table, Elf32_Shdr *section_headers)
{
	char *symbol_name = string_table + symbol->st_name;
	char symbol_type;

	print_symbol_type(symbol, &symbol_type, section_headers);

	if (symbol_type != 'U' && symbol_type != 'w')
		printf("%08x %c %s\n", symbol->st_value, symbol_type, symbol_name);
	else
		printf("         %c %s\n", symbol_type, symbol_name);
}

/**
 * parse_symbols32 - Parse and print all symbols in the symbol table.
 * @section_header: Pointer to the section header of the symbol table.
 * @symbol_table: Pointer to the symbol table.
 * @string_table: Pointer to the string table.
 * @section_headers: Array of section headers.
 */
void parse_symbols32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
char *string_table, Elf32_Shdr *section_headers)
{
	int symbol_count = section_header->sh_size / sizeof(Elf32_Sym);
	int i;

	for (i = 0; i < symbol_count; i++)
	{
		Elf32_Sym symbol = symbol_table[i];

		print_symbol32(&symbol, string_table, section_headers);
	}
}
