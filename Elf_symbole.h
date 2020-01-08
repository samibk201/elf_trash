#ifndef _H_SYMB
#define _H_SYMB
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
/***-Cette fonction initialise la table des symboles elle fait appel à
			deux fonctions de conversion du boutisme si besoin;
	-Données:-un fichier ouvert en lecture,une table de section et une entete elf;
			 -deux pointeurs (l'un pour le nombre de symbole puis l'autre l'indexe de symtab);
	-Résultat:-gestion faite assert en cas d'erreur;
			  -sinon un structure initialisée représentante de la table des symboles;
			  -modifie les deux variables nbsymb (nombre de symbole de la table) et index (indice de
			  de symtab dans la table des sections);
***/
Elf32_Sym *lecture_symb(FILE *felf,Elf32_Shdr *tab_section,Elf32_Ehdr elf_head,int *nbsymb,int *index);
/***-Cette fonction affiche la table des tables en faisant appel à d'autres fonctions auxiliaires;
	-Données: -deux structures l'une (table des symboles) initialisée et l'autre  (table des sections )
			  -le nombre de symbole puis l'indice le section symtab
			  -un fichier elf ouvert en lecture
	-Résultat: Aucun
***/
void affiche_symbole_table (Elf32_Sym *symbtab,int nbsymb,int index,Elf32_Shdr *tab_section,FILE *felf);
#endif
