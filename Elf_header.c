#include "Elf_header.h"
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <byteswap.h>
#include <assert.h>

// vérifie si le fichier est au format elf : -1 si le format n'est pas bon
int ElfId (Elf32_Ehdr * elf_head) {
	int resultat  = 0;
	if (elf_head->e_ident[EI_MAG0] != ELFMAG0 || elf_head->e_ident[EI_MAG1] != ELFMAG1 ||
        elf_head->e_ident[EI_MAG2] != ELFMAG2 || elf_head->e_ident[EI_MAG3] != ELFMAG3)
            resultat = -1;
	return resultat;
}
//lecture de l'entête elf pour initialiser la structure : NULL en cas d'erreur
Elf32_Ehdr *lecture_entete (FILE *felf) {
	Elf32_Ehdr *elf_head = malloc (sizeof(Elf32_Ehdr));
    int nmemb;
	if (elf_head == NULL) {
		return elf_head;
	}
    //rewind (felf);
    /************e_ident*********/
	assert(fread (elf_head->e_ident,1,EI_NIDENT,felf) == EI_NIDENT);
    if (ElfId (elf_head) < 0) {
        printf ("readelf:: ERREUR: N'est pas un fichier ELF – a les mauvais octets magiques au départ\n");
        elf_head = NULL;
        return elf_head;
    }
     /***********e_type***********/
    nmemb = fread (&(elf_head->e_type),sizeof(elf_head->e_type),1,felf);       assert(nmemb == 1);
    /***********e_machine********/
    nmemb = fread (&(elf_head->e_machine),sizeof(elf_head->e_machine),1,felf); assert(nmemb == 1);
    /***********e_version********/
    nmemb = fread (&(elf_head->e_version),sizeof(elf_head->e_version),1,felf); assert(nmemb == 1);
    /***********e_entry**********/
    nmemb = fread (&(elf_head->e_entry),sizeof(elf_head->e_entry),1,felf) ;    assert(nmemb == 1);
    /***********e_phoff**********/
    nmemb = fread (&(elf_head->e_phoff),sizeof(elf_head->e_phoff),1,felf);     assert(nmemb == 1);
    /***********e_shoff**********/
	nmemb = fread (&(elf_head->e_shoff),sizeof(elf_head->e_shoff),1,felf);     assert(nmemb == 1);
    /***********e_flags**********/
    nmemb = fread (&(elf_head->e_flags),sizeof(elf_head->e_flags),1,felf);     assert(nmemb == 1);
    /***********e_ehsize*********/
    nmemb = fread (&(elf_head->e_ehsize),sizeof(elf_head->e_ehsize),1,felf);   assert(nmemb == 1);
    /***********e_phentsize******/
    nmemb = fread (&(elf_head->e_phentsize),sizeof(elf_head->e_phentsize),1,felf);  assert(nmemb == 1);
    /***********e_phnum**********/
    nmemb = fread (&(elf_head->e_phnum),sizeof(elf_head->e_phnum),1,felf);          assert(nmemb == 1);
    /***********e_shentsize******/
    nmemb = fread (&(elf_head->e_shentsize),sizeof(elf_head->e_shentsize),1,felf);  assert(nmemb == 1);
    /***********e_shnum**********/
    nmemb = fread (&(elf_head->e_shnum),sizeof(elf_head->e_shnum),1,felf) ;     assert(nmemb == 1);
    /***********e_shstrndx*******/
    nmemb = fread (&(elf_head->e_shstrndx),sizeof(elf_head->e_shstrndx),1,felf);    assert(nmemb == 1);
    /*conversion en litle endian si le fichier est en big endian*/    
    if (elf_head->e_ident[EI_DATA] == ELFDATA2MSB) {
		elf_head -> e_type      = bswap_16 (elf_head -> e_type);
		elf_head -> e_machine   = bswap_16 (elf_head -> e_machine);
		elf_head -> e_version   = bswap_32 (elf_head -> e_version);
		elf_head -> e_entry     = bswap_32 (elf_head -> e_entry);
		elf_head -> e_phoff     = bswap_32 (elf_head -> e_phoff);
		elf_head -> e_shoff     = bswap_32 (elf_head -> e_shoff);
		elf_head -> e_flags     = bswap_32 (elf_head -> e_flags);
		elf_head -> e_ehsize    = bswap_16 (elf_head -> e_ehsize);
		elf_head -> e_phentsize = bswap_16 (elf_head -> e_phentsize);
		elf_head -> e_phnum     = bswap_16 (elf_head -> e_phnum);
		elf_head -> e_shentsize = bswap_16 (elf_head -> e_shentsize);
		elf_head -> e_shnum     = bswap_16 (elf_head -> e_shnum);
		elf_head -> e_shstrndx  = bswap_16 (elf_head -> e_shstrndx);
		
	}
    return elf_head;
}

