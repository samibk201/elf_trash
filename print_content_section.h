#ifndef __PRINT_CONTENT_SECTION_H__
#define __PRINT_CONTENT_SECTION_H__
#include <elf.h>
#include <stdio.h>
#include <ctype.h>
#define N 16

							/*  gère l'affichage du contenu d'une section   */




/***	Cette fonction test l' existance d' une section 
		Données : une chaine de caractere, table des sections, l'entete et le nom d'une section
		Résultat: entier(num de la section 'name' )
***/
int existe_section(unsigned char *, Elf32_Shdr*, Elf32_Ehdr*, char *);


/***	Cette fonction affiche le contenu brute d'une section
		Données : une chaine de caractere , table des sections, l'entete ,un fichier ELF et le nom d'une section
		Résultat: aucun
***/
void affiche_contentSection(unsigned char *, Elf32_Shdr*, Elf32_Ehdr*, FILE *, char*);



/***	Cette fonction est utilsée par la fonction lire_contentsec  pour faire l`affichage en format hexa
		Données : une addresse, un tampon et une taille
		Résultat: aucun
***/
void hexdump(int,unsigned char[],int);




/***	Cette fonction est utilsée par la fonction affiche_contentSection, permet de lire une chaine à partir d`une addresse 
		Données :  un fichier ELF, une adresse et une taille
		Résultat: aucun
***/
void lire_contentsec(FILE *,int,int);



/***	Cette fonction permet de lire le contenu d`une section 
		Données : nom d'une section, table des sections, l'entete , un fichier ELF et un boolean
		Résultat: chaine de caractére
***/
unsigned char *print_content_section(char*, Elf32_Shdr*, Elf32_Ehdr*, FILE *, int);


#endif
