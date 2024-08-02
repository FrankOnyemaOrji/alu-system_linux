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
