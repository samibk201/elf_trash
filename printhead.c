#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "printhead.h"


void print_e_ident(Elf32_Ehdr *entete)
{
    int i = 0;
    for(; i<16; i++)
        printf("%02x ", entete->e_ident[i]);
    printf("\n");
}

void print_type(Elf32_Ehdr *entete)
{
      switch (entete->e_type) {
		case ET_NONE:
			printf ("Aucun");
		 break;
		 case ET_REL:
			printf ("REL (Fichier de réadressage)");
		 break;
		 case ET_EXEC:
			printf ("EXEC (fichier exécutable)");
		 break;
		 case ET_DYN:
			printf ("DYN (fichier objet partagé)");
		 break;
		 case ET_CORE:
			printf ("CORE (fichier Core)");
		 break;
		 case ET_LOPROC:
			printf ("LOPROC");
		 break;
		 case ET_HIPROC:
			printf ("HIPROC");
		 break;
		 default:printf("Inconnue");
	}
	printf ("\n");
}

//affichage du type de classe
void print_classe_type(Elf32_Ehdr *entete)
{   
	switch (entete->e_ident[EI_CLASS]) {
		case ELFCLASSNONE: printf("Aucune");
			break;
		case ELFCLASS32: printf("ELF32");
			break;
		case ELFCLASS64:  printf("ELF64");
			break;
	}
	printf("\n");
}

//affichage du type de codage
void print_codage_type(Elf32_Ehdr *entete)
{
	switch (entete->e_ident[EI_DATA]) {
		case ELFDATANONE: printf("Aucune");
			break;
		case ELFDATA2LSB: printf("complément à 2, système à octets de poids faible d'abord (little endian)");
			break;
		case ELFDATA2MSB: printf ("complément à 2, système à octets de poids fort d'abord (big endian)");
			break;
	}
	printf("\n");
}

//affichhage architecture machine
void print_v_machine(Elf32_Ehdr *entete)
{
    switch(entete->e_machine)
    {
        case ET_NONE:   printf("No machine");
            break;
        case EM_M32:    printf("AT&T WE 32100");
            break;
        case EM_SPARC:  printf("SUN SPARC");
            break;
        case EM_386:    printf("Intel 80386");
            break;
        case EM_68K:    printf("Motorola m68k family");
            break;
        case EM_88K:    printf("Motorola m88k family");
            break;
        case EM_860:    printf("Intel 80860");
            break;
        case EM_MIPS:   printf("MIPS R3000 big-endian");
            break;
        case EM_MIPS_RS3_LE:   printf("MIPS RS4000 little-endian");
            break;
        case 62:               printf ("Advanced Micro Devices X86-64");
            break;
        case EM_ARM:           printf ("ARM");
            break;
        default:               printf("RESERVED");
            break;
    }
    printf("\n");
}

//print OS
void print_OS(Elf32_Ehdr *entete)
{
    if(entete->e_ident[EI_OSABI] == ELFOSABI_SYSV)
        printf("UNIX - System V");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_ARM)
        printf("ARM");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_LINUX)
        printf("LINUX - System");
    if(entete->e_ident[EI_OSABI] == ELFOSABI_TRU64)
        printf("UNIX TRU64");
    printf("\n");
}

//affichage version
void print_e_version(Elf32_Ehdr *entete)
{
	switch (entete->e_version) {
		case EV_NONE:     printf("0x0");
			break;
		case EV_CURRENT : printf("0x1");
			break;
	}
	printf("\n");
}

//affichage de la version courant
void print_version (Elf32_Ehdr *entete) {
	switch (entete->e_ident[EI_VERSION]) {
	case EV_NONE: printf ("0 (invadlid)");
		break;
	case EV_CURRENT: printf("1 (current)");
		break;
	}
	printf("\n");
}

void print_space (int n) {
    int i = 0;
    for (; i < n; i ++)
        printf (" ");
}
void print_flags (Elf32_Ehdr *entete) {
	switch(entete->e_flags) {
		case 0x05000000: printf ("Version5 EABI");
			break;
		case 0x00800000:	printf ("Version8 EABI");
			break;
		case 0x00400FFF: printf ("Version4 EABI and earlier");
			break;
		case 0x00000400: printf ("Version5 EABI and later");
			break;
		case 0x00000200: printf ("Version5 EABI and later");
			break;
		case 0x5000200: printf("Version5 EABI, soft-float ABI");
			break;
	}
	printf ("\n");
}
//affichage de l'entete
void print_header(Elf32_Ehdr *entete)
{
    printf("En-tête ELF:\n");
    printf("  Magique:"); print_space(3);  print_e_ident(entete);
    printf("  Classe:");  print_space(28); print_classe_type(entete);
    printf("  Données:"); print_space(26); print_codage_type(entete);
    printf("  Version:"); print_space(27); print_version(entete);
    printf("  OS/ABI:");  print_space(28); print_OS(entete);
    printf("  Version ABI:"); print_space(23); printf("%d\n", entete->e_ident[EI_ABIVERSION]);
    printf("  Type:");print_space(30); print_type(entete);
    printf("  Machine:"); print_space(27); print_v_machine(entete);
    printf("  Version:"); print_space(27); print_e_version(entete);
    printf("  Adresse du point d'entrée:"); print_space(15); printf("0x%0x\n", entete->e_entry);
    printf("  Début des en-têtes de programme :");print_space(10);printf("%d (octets dans le fichier)\n", entete->e_phoff);
    printf("  Début des en-têtes de section :");print_space(10);printf("%d (octets dans le fichier)\n", entete->e_shoff);
    printf("  Fanions:");print_space(27);printf("0x%0x, ", entete->e_flags);print_flags (entete);
    printf("  Taille de cet en-tête:");print_space(13);printf("%d (octets)\n", entete->e_ehsize);
    printf("  Taille de l'en-tête du programme:");print_space(2);printf("%d (octets)\n", entete->e_phentsize);
    printf("  Nombre d'en-tête du programme:");print_space(5);printf("%d\n", entete->e_phnum);
    printf("  Taille des en-têtes de section:");print_space(4);printf("%d (octets)\n", entete->e_shentsize);
    printf("  Nombre d'en-têtes de section:");print_space(6);printf("%d\n", entete->e_shnum);
    printf("  Table d'indexes des chaînes d'en-tête de section:");print_space(1);printf("%d\n", entete->e_shstrndx);
}
