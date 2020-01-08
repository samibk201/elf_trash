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
#include "fusion.h"
#include "reloc_table.h"


int main (int argc, char** argv){
	if (argc != 4){
        fprintf(stderr, "Nombre d'arguments incorrect\n");
        return EXIT_FAILURE;
    }
    
    FILE* file_in1 = fopen(argv[1], "rb");
   	if (file_in1 == NULL) {
		fprintf(stdout, "Erreur d'ouverture du premier fichier \n");
		return EXIT_FAILURE;		
	}


    FILE* file_in2 = fopen(argv[2], "rb");
   	if (file_in2 == NULL) {
		fprintf(stdout, "Erreur d'ouverture du second fichier \n");
		return EXIT_FAILURE;		
	}

	FILE* file_out = fopen(argv[3], "wb");
   	if (file_out == NULL) {
		fprintf(stdout, "Erreur d'ouverture du fichier de sorti\n");
		return EXIT_FAILURE;		
	}




    Elf32_Ehdr *elf_head1,*elf_head2;
	Elf32_Shdr *sections_table1,*sections_table2;



    elf_head1 = lecture_entete(file_in1);
    elf_head2 = lecture_entete(file_in2);


    sections_table1=malloc(elf_head1->e_shnum*sizeof(Elf32_Shdr));
    get_sh_values(&sections_table1 , file_in1 , elf_head1);

    sections_table2=malloc(elf_head2->e_shnum*sizeof(Elf32_Shdr));
    get_sh_values(&sections_table2 , file_in2 , elf_head2);



    Donnees* d = malloc(sizeof(Donnees));


    init_fusion(d, sections_table1, elf_head1,  sections_table2, elf_head2);

    /* fusion PROGBITS , REL,  ...*/
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_NULL);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_PROGBITS);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_NOBITS);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_REL);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_RELA);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_SYMTAB);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_STRTAB);
    fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,SHT_ARM_ATTRIBUTES);

    modification_indx_sections(d);


    /* example d'affichage des offsets */
    // for (int i = 0; i < d->nbS1; ++i){
  		//  if((d->f[i].type== SHT_NULL)||(d->f[i].type== SHT_NOBITS)||(d->f[i].type== SHT_PROGBITS)||(d->f[i].type== SHT_REL)) {
  		// 	printf("************* section %d ***************\n",i);
				// printf("%06x\n",d->f[i].newsh[0].sh_offset);
    //  	    if(d->f[i].nbS==2)
				//       {printf("%06x\n",d->f[i].newsh[1].sh_offset);}
			 //  printf("*****************************\n");
    //  	}	
    // }

    for (int i = 0; i < d->nbS1; ++i){
          printf("************* section %d ***************\n",i);
          printf("%d\n",d->f[i].sh_link);
          printf("%d\n",d->f[i].sh_info);
          printf("*****************************\n");
    }




  freemem (elf_head1);
  freemem (elf_head2);
	freemem (sections_table1);
	freemem (sections_table2);
	fclose (file_in1);
	fclose (file_in2);
  fclose (file_out);
	freemem(d->sh1);
	freemem(d->sh2);
	for (int i = 0; i < d->nbS1; ++i){
		freemem(d->f[i].newsh);
	}
	freemem(d->f);
	freemem(d);


    return EXIT_SUCCESS;
}

