#include "hnm.h"

/**
 * check_elf_header - Check the ELF header for validity.
 * @file: Pointer to the ELF file.
 * @elf_header: Pointer to the ELF header.
 * @file_path: Path to the ELF file.
 *
 * Return: 0 if successful, -1 otherwise.
 */
int check_elf_header(FILE *file, Elf32_Ehdr *elf_header, char *file_path)
{
	fread(elf_header, sizeof(Elf32_Ehdr), 1, file);

	if (elf_header->e_ident[EI_CLASS] != ELFCLASS32)
	{
		handle_elf_format_error(file_path);
		fclose(file);
		return (-1);
	}

	if (elf_header->e_ident[EI_DATA] != ELFDATA2LSB &&
	    elf_header->e_ident[EI_DATA] != ELFDATA2MSB)
	{
		handle_elf_endianness_error(file_path);
		fclose(file);
		return (-1);
	}

	return (0);
}

/**
 * process_elf_file32 - Process the ELF file and print the symbols.
 * @file_path: Path to the ELF file.
 */
void process_elf_file32(char *file_path)
{
	FILE *file = fopen(file_path, "rb");
	Elf32_Ehdr elf_header;
	Elf32_Shdr *section_headers;
	int symbol_table_index;
	Elf32_Sym *symbol_table;
	char *string_table;

	if (file == NULL)
	{
		handle_file_open_error(file_path);
		return;
	}

	if (check_elf_header(file, &elf_header, file_path) == -1)
	{
		return;
	}

	read_section_headers32(file, &elf_header, &section_headers);

	symbol_table_index = find_symbol_table_index(&elf_header, section_headers);

	if (symbol_table_index == -1)
	{
		fprintf(stderr, "No symbols in file: %s\n", file_path);
		fclose(file);
		free(section_headers);
		return;
	}

	read_symbol_and_string_tables32(file, section_headers, &symbol_table,
				     &string_table, symbol_table_index);

	parse_symbols32(&section_headers[symbol_table_index], symbol_table,
			string_table, section_headers);

	fclose(file);
	free(section_headers);
	free(symbol_table);
	free(string_table);
}

