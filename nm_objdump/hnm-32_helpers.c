#include "hnm.h"

/**
 * read_section_headers32 - Read the section headers from the ELF file.
 * @file: Pointer to the ELF file.
 * @elf_header: Pointer to the ELF header.
 * @section_headers: Double pointer to store the section headers.
 */
void read_section_headers32(FILE *file, Elf32_Ehdr *elf_header,
Elf32_Shdr **section_headers)
{
	*section_headers = malloc(elf_header->e_shentsize * elf_header->e_shnum);
	if (*section_headers == NULL)
	{
		fprintf(stderr, "Memory allocation error for section headers\n");
		exit(1);
	}

	fseek(file, elf_header->e_shoff, SEEK_SET);
	fread(*section_headers, elf_header->e_shentsize, elf_header->e_shnum, file);
}

/**
 * read_symbol_and_string_tables32 - Read the symbol and string tables
 *                                   from the ELF file.
 * @file: Pointer to the ELF file.
 * @section_headers: Array of section headers.
 * @symbol_table: Double pointer to store the symbol table.
 * @string_table: Double pointer to store the string table.
 * @symbol_table_index: Index of the symbol table section.
 */
void read_symbol_and_string_tables32(FILE *file, Elf32_Shdr *section_headers,
Elf32_Sym **symbol_table, char **string_table, int symbol_table_index)
{
	Elf32_Shdr symbol_table_header = section_headers[symbol_table_index];
	int string_table_index;
	Elf32_Shdr string_table_header;

	*symbol_table = malloc(symbol_table_header.sh_size);
	fseek(file, symbol_table_header.sh_offset, SEEK_SET);
	fread(*symbol_table, symbol_table_header.sh_size, 1, file);

	string_table_index = symbol_table_header.sh_link;
	string_table_header = section_headers[string_table_index];

	*string_table = malloc(string_table_header.sh_size);
	fseek(file, string_table_header.sh_offset, SEEK_SET);
	fread(*string_table, string_table_header.sh_size, 1, file);
}
