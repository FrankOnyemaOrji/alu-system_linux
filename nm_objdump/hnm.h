#ifndef HNM_H
#define HNM_H

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <ctype.h>

void print_symbol_table64(Elf64_Shdr *section_header, Elf64_Sym *symbol_table,
			  char *string_table, Elf64_Shdr *section_headers);
void process_elf_file64(char *file_path);
void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
			  char *string_table, Elf32_Shdr *section_headers);
void process_elf_file32(char *file_path);
char get_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers);
char get_section_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers);
char get_weak_symbol_type(Elf32_Sym symbol);
char get_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers);
char get_section_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers);
void print_symbol(Elf32_Sym symbol, char *symbol_name, char symbol_type);
void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
                          char *string_table, Elf32_Shdr *section_headers);
void process_elf_file32(char *file_path);
int validate_elf_file(Elf32_Ehdr *elf_header, char *file_path);
int find_symbol_table(Elf32_Shdr *section_headers, int shnum);
void cleanup(FILE *file, void *section_headers, void *symbol_table,
             void *string_table);
#endif /* HNM_H */
