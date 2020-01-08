//GROUPE 13
//BAH    MAMADOU
//HAFID  MOHAMED
//BELAID MOHAMED
//DAN  	 MUSHINGA
//BAH    AMADOU
//AMANI  BELAID

#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>
#include "printhead.h"
#include <unistd.h>
#include <string.h>
#include "section_header.h"
#include "print_content_section.h"
#include "Elf_symbole.h"
#include "print_symbole.h"
#include "reloc_table.h"

#define A  0 //toutes les commandes
#define H  1 //entete
#define X  2 //une section avec numéro ou name
#define XS 3 //table section
#define R  4 //relocation
#define S  5 //symbole

void freemem (void *adr) {
	if (adr != NULL) free (adr);
}

int main (int argc,char **argv) { 
	int option=0,choix=0;
	while ((option=getopt(argc,argv,"ahxsrS:")) != -1) {
		switch (option) {
			case 'a':
					//all
					choix = A;
				break;
			case 'h':
					//entete
					choix = H;
				break;
			case 'x':
					//une section
					choix = X;
				break;
			case 's':
					//symbole
					choix = S;
				break;
			case 'r':
					//relocation
					choix = R;
				break;
			case 'S':
					//table section
					choix = XS;
				break;
			default:
				return EXIT_FAILURE;
				
		}
	}
	FILE *felf;
	if (choix == X) {
		//pour l'option -x le nom du fichier correspont à la variable argv [3]
		if(argc < 4) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		felf = fopen (argv[3],"rb");
	} else {
		//pour les autres options  le nom du fichier correspont à la variable argv [2]
		if(argc < 3) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		felf = fopen (argv[2],"rb");
	}
	if (felf == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier \n");
		return EXIT_FAILURE;		
	}

	char *namesection;
	unsigned char *strtab=malloc(sizeof(unsigned char*));

	//déclaration des tructures
	Elf32_Ehdr * elf_head;
	Elf32_Shdr *sectionHeader, *tabSection, *REL_tab, *SYM_tab;
	Elf32_Sym *symbtab;

	int nbsymb = 0, index = 0;

	//initalisation de la structure de l'entete
	elf_head = lecture_entete (felf);
	if (elf_head == NULL)
	{
		fprintf(stderr, "Erreur d'initialisation de l'entete \n");
		exit(1);
	}
	tabSection = malloc(elf_head->e_shnum*sizeof(Elf32_Shdr));
	REL_tab = malloc(elf_head->e_shnum*sizeof(Elf32_Shdr));
	SYM_tab = malloc(elf_head->e_shnum*sizeof(Elf32_Shdr));
	sectionHeader = malloc(elf_head->e_shnum*sizeof(Elf32_Shdr));

	//initialisation de la table des sections
	get_sh_values(&sectionHeader,felf,elf_head);
	//initalisation de la structure correspondante à la table des symboles
	symbtab = lecture_symb(felf,sectionHeader,*elf_head,&nbsymb,&index);

	//exécution commande option
	switch (choix) {
		case A:
			//all : 
			print_header(elf_head);
			print_section(felf,sectionHeader,elf_head);
			break;
		case XS:
			//table section : 
			print_section(felf,sectionHeader,elf_head);
			break;
		case H:
			//entete :
			print_header(elf_head);
			break;
		case S:
			//symbole :
			affiche_symbole_table (symbtab,nbsymb,index,sectionHeader,felf);
			break;
		case X://le contenu d'une section
			namesection=malloc(sizeof(char)*strlen(argv[2])+1);
			if (namesection == NULL) 
				return EXIT_FAILURE;
			strcpy(namesection,"");
			strcat(namesection,argv[2]);
			strtab=print_content_section(namesection, sectionHeader, elf_head, felf, 1);
			break;

		case R:
			//relocation
			namesection=malloc(sizeof(char)*strlen(argv[2])+1);
			if (namesection == NULL) 
				return EXIT_FAILURE;
			strcpy(namesection,"");
			strcat(namesection,argv[2]);
			strtab=print_content_section(namesection, sectionHeader, elf_head, felf, 0);
			affiche_Relocation(sectionHeader, *elf_head, symbtab, strtab, felf);

	}

	//libérationde toutes strtuctures allouées
	freemem (elf_head);
	freemem (sectionHeader);
	freemem(tabSection);
	freemem(SYM_tab);
	freemem(REL_tab);
	//fermeture du fichier ouvert en lecture
	fclose (felf);

	return EXIT_SUCCESS;
}
