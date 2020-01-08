#ifndef _H_RELOC
#define _H_RELOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <assert.h>
#include "Elf_header.h"
#include "section_header.h"



Elf32_Rel *read_REL_tab(Elf32_Ehdr h, Elf32_Shdr *S, FILE *file, int index);

void print_RelTab_head(int RelSize, unsigned char *sh_name, unsigned int sh_offset);

char * reloc_types(Elf32_Rel REL_tab);

char* get_sym_type (unsigned int type);

void affiche_Relocation(Elf32_Shdr*Sec, Elf32_Ehdr h, Elf32_Sym *Sym, unsigned char *strtab, FILE *file);

char* SymName(Elf32_Shdr *Sec, int index, int indexStrTab, FILE *file);

#endif
