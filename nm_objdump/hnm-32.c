#include "hnm.h"

/**
 * open_elf_file - Open the ELF file
 * @file_path: Path to the ELF file
 * Return: FILE pointer to the opened file
 */
FILE *open_elf_file(char *file_path)
{
	FILE *file = fopen(file_path, "rb");

	if (file == NULL)
	{
		fprintf(stderr, "./hnm: %s: failed to open file\n", file_path);
	}
	return (file);
}

/**
 * read_elf_header - Read the ELF header from the file
 * @file: FILE pointer to the opened ELF file
 * @elf_header: Pointer to the ELF header structure
 * Return: 0 on success, -1 on failure
 */
int read_elf_header(FILE *file, Elf32_Ehdr *elf_header)
{
	fread(elf_header, sizeof(Elf32_Ehdr), 1, file);

	if (elf_header->e_ident[EI_CLASS] != ELFCLASS32 &&
		elf_header->e_ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "./hnm: unsupported ELF file format\n");
		return (-1);
	}

	int is_little_endian = (elf_header->e_ident[EI_DATA] == ELFDATA2LSB);
	int is_big_endian = (elf_header->e_ident[EI_DATA] == ELFDATA2MSB);

	if (!is_little_endian && !is_big_endian)
	{
		fprintf(stderr, "./hnm: unsupported ELF file endianness\n");
		return (-1);
	}

	return (0);
}

/**
 * read_section_headers - Read the section headers from the file
 * @file: FILE pointer to the opened ELF file
 * @elf_header: Pointer to the ELF header structure
 * @section_headers: Pointer to store the section headers array
 * Return: 0 on success, -1 on failure
 */
int read_section_headers(FILE *file, Elf32_Ehdr *elf_header,
	Elf32_Shdr **section_headers)
{
	*section_headers = malloc(elf_header->e_shentsize * elf_header->e_shnum);
	if (*section_headers == NULL)
	{
		fprintf(stderr, "./hnm: memory allocation error for section_headers\n");
		return (-1);
	}

	fseek(file, elf_header->e_shoff, SEEK_SET);
	fread(*section_headers, elf_header->e_shentsize, elf_header->e_shnum, file);
	return (0);
}

/**
 * find_symbol_table_index - Find the symbol table index in the section headers
 * @elf_header: Pointer to the ELF header structure
 * @section_headers: Pointer to the array of section headers
 * Return: Index of the symbol table, -1 if not found
 */
int find_symbol_table_index(Elf32_Ehdr *elf_header,
Elf32_Shdr *section_headers)
{
	for (int i = 0; i < elf_header->e_shnum; i++)
	{
		if (section_headers[i].sh_type == SHT_SYMTAB)
		{
			return (i);
		}
	}
	return (-1);
}
