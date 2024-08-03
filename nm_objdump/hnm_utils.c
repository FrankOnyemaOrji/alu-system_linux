#include "hnm.h"

/**
 * print_symbol_table32 - Print the symbol table for 32-bit ELF files
 * @symbol_table_header: Pointer to the symbol table header
 * @symbol_table: Pointer to the symbol table array
 * @string_table: Pointer to the string table
 * @section_headers: Pointer to the section headers
 */
void print_symbol_table32(Elf32_Shdr *symbol_table_header,
Elf32_Sym *symbol_table, char *string_table,
Elf32_Shdr *section_headers)
{
	for (int i = 0; i < symbol_table_header->sh_size / sizeof(Elf32_Sym); i++)
	{
		Elf32_Sym *symbol = &symbol_table[i];
		char *symbol_name = &string_table[symbol->st_name];

		printf("%08x %02x %02x %04x %04x %s\n",
		       symbol->st_value, symbol->st_info, symbol->st_other,
		       symbol->st_shndx, symbol->st_size, symbol_name);
	}
}

/**
 * load_elf_components - Load ELF components
 * @file: Pointer to the opened ELF file
 * @elf_header: Pointer to store the ELF header
 * @section_headers: Pointer to store the section headers
 * @symbol_table: Pointer to store the symbol table
 * @string_table: Pointer to store the string table
 * @file_path: Path to the ELF file
 * Return: 0 on success, -1 on failure
 */
int load_elf_components(FILE *file, Elf32_Ehdr *elf_header,
			Elf32_Shdr **section_headers,
			Elf32_Sym **symbol_table, char **string_table,
			char *file_path)
{
	if (read_elf_header(file, elf_header) != 0 ||
	    read_section_headers(file, elf_header, section_headers) != 0)
		return (-1);

	int symbol_table_index = find_symbol_table_index
(elf_header, *section_headers);

	if (symbol_table_index == -1)
	{
		fprintf(stderr, "./hnm: %s: no symbols\n", file_path);
		return (-1);
	}

	if (read_symbol_table(file, *section_headers,
symbol_table_index, symbol_table) != 0)
		return (-1);

	int string_table_index = (*section_headers)[symbol_table_index].sh_link;

	if (read_string_table(file, *section_headers,
string_table_index, string_table) != 0)
	{
		free(*symbol_table);
		return (-1);
	}

	return (0);
}

/**
 * load_elf_file - Load the ELF file and its components
 * @file_path: Path to the ELF file
 * @file: Pointer to store the FILE pointer to the opened ELF file
 * @elf_header: Pointer to store the ELF header
 * @section_headers: Pointer to store the section headers
 * @symbol_table: Pointer to store the symbol table
 * @string_table: Pointer to store the string table
 * Return: 0 on success, -1 on failure
 */
int load_elf_file(char *file_path, FILE **file, Elf32_Ehdr *elf_header,
		  Elf32_Shdr **section_headers, Elf32_Sym **symbol_table,
		  char **string_table)
{
	*file = open_elf_file(file_path);
	if (*file == NULL)
		return (-1);

	if (load_elf_components(*file, elf_header, section_headers,
				symbol_table, string_table, file_path) != 0)
	{
		fclose(*file);
		return (-1);
	}

	return (0);
}

/**
 * process_elf_file32 - program that processes a 32-bit ELF file
 * located at the given file path
 * @file_path: a pointer to a string that contains the path to the ELF file
 * Return: nothing (void)
 */
void process_elf_file32(char *file_path)
{
	FILE *file;
	Elf32_Ehdr elf_header;
	Elf32_Shdr *section_headers;
	Elf32_Sym *symbol_table;
	char *string_table;

	if (load_elf_file(file_path, &file, &elf_header, &section_headers,
			  &symbol_table, &string_table) != 0)
		return;

	print_symbol_table32(&section_headers[find_symbol_table_index(&elf_header,
		section_headers)], symbol_table, string_table, section_headers);

	fclose(file);
	free(section_headers);
	free(symbol_table);
	free(string_table);
}
