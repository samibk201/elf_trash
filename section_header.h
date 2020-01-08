#ifndef _H_SECH_
#define _H_SECH_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <byteswap.h>
#include <assert.h>
#include "Elf_header.h"

#define SIZENAME 30
//-entrees : une entete h et un entier sur 32 bits value,
//-sortie : l'entier value en little endian
int32_t val_32(Elf32_Ehdr h, int32_t value);

//-entrees : une entete h et un entier sur 16 bits value,
//-sortie : l'entier value en little endian
int16_t val_16(Elf32_Ehdr h, int16_t value);


//-entrees: un fichier, une section et une entete
//-sortie: NULL
//-la fonction affecte les bonne valeurs (en little endian) de la section du fichier f a la section s
void read_section(FILE *fichier, Elf32_Shdr *s, Elf32_Ehdr h);

//retourne la chaine de characteres correspondate au type de la section s
char *read_type (Elf32_Shdr s);

//retourne la chaine de characteres correspondate au flag de la section s
char *read_flags (char *flag, Elf32_Word s);

//pour afficher la section
void print_section (FILE *file, Elf32_Shdr *T, Elf32_Ehdr *e);

//la fonction recupere toutes les valeurs de la section en little endian et les met dans sectionHeader
void get_sh_values(Elf32_Shdr **sectionHeader, FILE* f,Elf32_Ehdr *h);

#endif







