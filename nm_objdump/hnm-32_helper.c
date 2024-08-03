#include "hnm.h"

/**
 * print_symbol_table32 - program that prints the symbol
 * table for a 32-bit ELF file
 * considering special section indices and visibility attributes
 * @section_header: a pointer to the section header of the symbol table
 * @symbol_table: a pointer to the beginning of the symbol table
 * @string_table: a pointer to the beginning of the string table,
 *                which contains the names of the symbols
 * @section_headers: a pointer to the array of section headers for the ELF file
 * Return: nothing (void)
 * Author: Frank Onyema Orji
 */

void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
			  char *string_table, Elf32_Shdr *section_headers)
{
	int i;
	int symbol_count = section_header->sh_size / sizeof(Elf32_Sym);
	char *symbol_name;
	char symbol_type;

	/* Adding a blank line after declarations */
	for (i = 0; i < symbol_count; i++)
	{
		Elf32_Sym symbol = symbol_table[i];

		symbol_name = string_table + symbol.st_name;

		if (symbol.st_name != 0 && ELF32_ST_TYPE(symbol.st_info) != STT_FILE)
		{
			symbol_type = get_symbol_type32(symbol, section_headers);

			if (ELF32_ST_BIND(symbol.st_info) == STB_LOCAL)
			{
				symbol_type = tolower(symbol_type);
			}

			print_symbol32(symbol, symbol_name, symbol_type);
		}
	}
}

/**
 * get_symbol_type32 - helper function to get the symbol type
 * @symbol: the symbol to get the type of
 * @section_headers: a pointer to the array of section headers for the ELF file
 * Return: the symbol type character
 */
char get_symbol_type32(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	char symbol_type = '?';

	if (ELF32_ST_BIND(symbol.st_info) == STB_WEAK)
	{
		symbol_type = get_weak_symbol_type32(symbol);
	}
	else if (symbol.st_shndx == SHN_UNDEF)
	{
		symbol_type = 'U';
	}
	else if (symbol.st_shndx == SHN_ABS)
	{
		symbol_type = 'A';
	}
	else if (symbol.st_shndx == SHN_COMMON)
	{
		symbol_type = 'C';
	}
	else if (symbol.st_shndx < SHN_LORESERVE)
	{
		symbol_type = get_regular_symbol_type32(symbol, section_headers);
	}

	return (symbol_type);
}

/**
 * get_weak_symbol_type32 - helper function to get the type of a weak symbol
 * @symbol: the weak symbol to get the type of
 * Return: the weak symbol type character
 */
char get_weak_symbol_type32(Elf32_Sym symbol)
{
	char symbol_type = '?';

	if (symbol.st_shndx == SHN_UNDEF)
	{
		symbol_type = 'w';
	}
	else if (ELF32_ST_TYPE(symbol.st_info) == STT_OBJECT)
	{
		symbol_type = 'V';
	}
	else
	{
		symbol_type = 'W';
	}

	return (symbol_type);
}

/**
 * get_regular_symbol_type32 - helper function
 * to get the type of a regular symbol
 * @symbol: the regular symbol to get the type of
 * @section_headers: a pointer to the array of section headers for the ELF file
 * Return: the regular symbol type character
 */
char get_regular_symbol_type32(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	char symbol_type = '?';
	Elf32_Shdr symbol_section = section_headers[symbol.st_shndx];

	if (ELF32_ST_BIND(symbol.st_info) == STB_GNU_UNIQUE)
	{
		symbol_type = 'u';
	}
	else if (symbol_section.sh_type == SHT_NOBITS &&
		 symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
	{
		symbol_type = 'B';
	}
	else if (symbol_section.sh_type == SHT_PROGBITS)
	{
		if (symbol_section.sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		{
			symbol_type = 'T';
		}
		else if (symbol_section.sh_flags == SHF_ALLOC)
		{
			symbol_type = 'R';
		}
		else if (symbol_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
		{
			symbol_type = 'D';
		}
	}
	else if (symbol_section.sh_type == SHT_DYNAMIC)
	{
		symbol_type = 'D';
	}
	else
	{
		symbol_type = 't';
	}

	return (symbol_type);
}

/**
 * print_symbol32 - helper function to print a symbol
 * @symbol: the symbol to print
 * @symbol_name: the name of the symbol
 * @symbol_type: the type of the symbol
 * Return: nothing (void)
 */
void print_symbol32(Elf32_Sym symbol, char *symbol_name, char symbol_type)
{
	if (symbol_type != 'U' && symbol_type != 'w')
	{
		printf("%08x %c %s\n", symbol.st_value, symbol_type, symbol_name);
	}
	else
	{
		printf("         %c %s\n", symbol_type, symbol_name);
	}
}
