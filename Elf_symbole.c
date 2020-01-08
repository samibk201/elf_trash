#include "Elf_symbole.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "section_header.h"
#include <byteswap.h>
#include "print_symbole.h"

//initialisation de la table des symboles,le nombre de symbole et l'indice de la section symtab
Elf32_Sym *lecture_symb(FILE *felf,Elf32_Shdr *tab_section,Elf32_Ehdr elf_head,int *nbsymb,int *index) {
	*index = 0;
	Elf32_Sym *symbtab;
	
	//recherche de l'indice de la section symtab
	while (*index < elf_head.e_shnum && tab_section [*index].sh_type != SHT_SYMTAB) {
		*index +=1;
	}
	//calcul du nombre de symbole
	*nbsymb = tab_section [*index].sh_size / 16; assert (*nbsymb != -1);
	//allocation de symbtab
	symbtab = malloc (sizeof(Elf32_Sym)*(*nbsymb)); assert(symbtab != NULL);
	
	int i = 0,nmemb;
	for (; i < *nbsymb ; i ++) {
		
		//réplacement au bon endroit dans le fichier
	    fseek (felf,tab_section[*index].sh_offset + i * tab_section [*index].sh_entsize,SEEK_SET);

		nmemb = fread (&symbtab[i].st_name,sizeof(symbtab[i].st_name),1,felf);   assert(nmemb == 1);
		symbtab[i].st_name  = val_32(elf_head,symbtab[i].st_name);
		nmemb = fread (&symbtab[i].st_value,sizeof(symbtab[i].st_value),1,felf); assert(nmemb == 1);
		symbtab[i].st_value = val_32(elf_head,symbtab[i].st_value);
		nmemb = fread (&symbtab[i].st_size,sizeof(symbtab[i].st_size),1,felf);   assert(nmemb == 1);
		symbtab[i].st_size = val_32(elf_head,symbtab[i].st_size);
		nmemb = fread (&symbtab[i].st_info,sizeof(symbtab[i].st_info),1,felf);   assert(nmemb == 1);
		nmemb = fread (&symbtab[i].st_other,sizeof(symbtab[i].st_other),1,felf); assert(nmemb == 1);
		nmemb = fread (&symbtab[i].st_shndx,sizeof(symbtab[i].st_shndx),1,felf); assert(nmemb == 1);
		nmemb =symbtab[i].st_shndx = val_16(elf_head,symbtab[i].st_shndx);
	}
	
	return symbtab;
}

//affichage de la table des symboles
void affiche_symbole_table (Elf32_Sym *symbtab,int nbsymb,int index,Elf32_Shdr *tab_section,FILE *felf) {
	
	char *tampon ;
	int linksecsym,nmemb;

	linksecsym = tab_section [index].sh_link;
	tampon = malloc(tab_section[linksecsym].sh_size);  	         assert (tampon != NULL);
	nmemb  = fseek(felf,tab_section[linksecsym].sh_offset,SEEK_SET); assert (nmemb != -1);
	nmemb  = fread (tampon,tab_section[linksecsym].sh_size,1,felf);	 assert (nmemb == 1);

	printf("\n");
	printf ("La table de symboles « .symtab » contient %d entrées :\n",nbsymb);
	printf ("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
	int i = 0;
	for (; i < nbsymb; i ++) {
		printf("   %3d: ", i);
		printf("%08x  ", symbtab[i].st_value);
		printf("%4d ", symbtab[i].st_size);
		print_type_symb(symbtab[i].st_info);
		print_link(symbtab[i].st_info);
		print_visi(symbtab[i].st_other);
		switch (symbtab[i].st_shndx) {
			case SHN_UNDEF: printf ("UND ");
				break;
			case SHN_ABS:   printf ("ABS ");
				break;
			default :
				printf("%3d ", symbtab[i].st_shndx);
		}
		printf("%s", tampon+symbtab[i].st_name);
		printf("\n");
	}
}
