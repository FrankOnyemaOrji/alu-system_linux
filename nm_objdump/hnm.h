#ifndef HNM_H
#define HNM_H

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <ctype.h>

void print_symbol_type(Elf32_Sym *symbol, char *symbol_type, Elf32_Shdr *section_headers);
void print_symbol32(Elf32_Sym *symbol, char *string_table, Elf32_Shdr *section_headers);
void parse_symbols32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table, 
                     char *string_table, Elf32_Shdr *section_headers);
void read_section_headers32(FILE *file, Elf32_Ehdr *elf_header, Elf32_Shdr **section_headers);
void read_symbol_and_string_tables32(FILE *file, Elf32_Shdr *section_headers, 
                                     Elf32_Sym **symbol_table, char **string_table, 
                                     int symbol_table_index);
void process_elf_file32(char *file_path);

/* Error handling functions */
void handle_file_open_error(char *file_path);
void handle_elf_format_error(char *file_path);
void handle_elf_endianness_error(char *file_path);

/* ELF processing functions */
int check_elf_header(FILE *file, Elf32_Ehdr *elf_header, char *file_path);
int find_symbol_table_index(Elf32_Ehdr *elf_header, Elf32_Shdr *section_headers);
void process_elf_file32(char *file_path);

#endif
