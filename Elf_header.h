#ifndef _H_ELF_HEADER_
#define _H_ELF_HEADER_
#include <elf.h>
#include <stdio.h>
#include "util.h"

/***-initialise la table d'entete elf et fait appel à la fonction ElfId;
    -Donnees: Elle prend un fichier ouvert en lecture;
    -Resultat: 
    		 - NULL si la structure n'a pas été initialisée (fichier non elf ou erreur allocation)
    		 - une structure (Elf32_Ehdr *) initialisée ;
***/
Elf32_Ehdr * lecture_entete (FILE *felf);
/***-test du format du fichier
	-Donées: Une structure (entete elf) initialisée;
	-Résultat: 
			- -1 si la structure n'est pas au format elf c'est à dire que les nombres magiques ne 
			  correspondent pas au format attendu ou que la classe n'est pas de ELF32;
			- 0 si le format est celui attendu;
***/
int ElfId (Elf32_Ehdr * elf_head);
#endif 
