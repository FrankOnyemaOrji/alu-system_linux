#include "hnm.h"

/**
 * handle_file_open_error - Handle the file open error.
 * @file_path: Path to the ELF file.
 */
void handle_file_open_error(char *file_path)
{
	fprintf(stderr, "Failed to open file: %s\n", file_path);
}

/**
 * handle_elf_format_error - Handle unsupported ELF file format errors.
 * @file_path: Path to the ELF file.
 */
void handle_elf_format_error(char *file_path)
{
	fprintf(stderr, "Unsupported ELF file format: %s\n", file_path);
}

/**
 * handle_elf_endianness_error - Handle unsupported ELF file endianness errors.
 * @file_path: Path to the ELF file.
 */
void handle_elf_endianness_error(char *file_path)
{
	fprintf(stderr, "Unsupported ELF file endianness: %s\n", file_path);
}

/**
 * find_symbol_table_index - Find the index of the symbol table section.
 * @elf_header: Pointer to the ELF header.
 * @section_headers: Array of section headers.
 *
 * Return: Index of the symbol table section, or -1 if not found.
 */
int find_symbol_table_index(Elf32_Ehdr *elf_header,
Elf32_Shdr *section_headers)
{
	int i;

	for (i = 0; i < elf_header->e_shnum; i++)
	{
		if (section_headers[i].sh_type == SHT_SYMTAB)
		{
			return (i);
		}
	}

	return (-1);
}
