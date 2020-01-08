#include "reloc_table.h"

Elf32_Rel *read_REL_tab(Elf32_Ehdr h, Elf32_Shdr *S, FILE *file, int index){
		int i;
		int RelSize=S[index].sh_size/S[index].sh_entsize;
		Elf32_Rel *REL_tab = malloc(sizeof(Elf32_Rel)*RelSize);
		fseek(file, S[index].sh_offset, SEEK_SET);

		for(i=0;i<RelSize;i++){
			int f __attribute__((unused)) = fread(&REL_tab[i],1,sizeof(Elf32_Rel),file);
			REL_tab[i].r_offset  = val_32(h, REL_tab[i].r_offset);	
			REL_tab[i].r_info    = val_32(h, REL_tab[i].r_info);
		}
	return REL_tab;
} 


char *reloc_types(Elf32_Rel REL_tab){
	char *type = malloc(SIZENAME*sizeof(char));
	switch(ELF32_R_TYPE(REL_tab.r_info)){
						
		case R_ARM_NONE:
			type="R_ARM_NONE";
			break;
		
		case R_ARM_PC24:
			type="R_ARM_PC24";
			break;
			
		case R_ARM_ABS32:
			type="R_ARM_ABS32";
			break;
		
		case R_ARM_REL32:
			type="R_ARM_REL32";
			break;
		
		case R_ARM_PC13:
			type="R_ARM_PC13";
			break;
		
		case R_ARM_ABS16:
			type="R_ARM_ABS16";
			break;
		
		case R_ARM_ABS12:
			type="R_ARM_ABS12";
			break;
		
		case R_ARM_THM_ABS5:
			type="R_ARM_THM_ABS5";
			break;

		case R_ARM_ABS8:
			type="R_ARM_ABS8";
			break;

		case R_ARM_SBREL32:
			type="R_ARM_SBREL32";
			break;

		case R_ARM_THM_PC22:
			type="R_ARM_THM_PC22";
			break;

		case R_ARM_THM_PC8:
			type="R_ARM_THM_PC8";
			break;

		case R_ARM_AMP_VCALL9:
			type="R_ARM_AMP_VCALL9";
			break;

		case R_ARM_SWI24:
			type="R_ARM_SWI24";
			break;

		case R_ARM_THM_SWI8:
			type="R_ARM_THM_SWI8";
			break;

		case R_ARM_XPC25:
			type="R_ARM_XPC25";
			break;

		case R_ARM_THM_XPC22:
			type="R_ARM_THM_XPC22";
			break;

		case R_ARM_CALL:
			type="R_ARM_CALL";
			break;

		case R_ARM_JUMP24:
			type="R_ARM_JUMP24";
			break;

		default:
  			type = "UNDEFINED";
			break;
	}
	return type;
}


char* get_sym_type (unsigned int type){
  switch (type)
    {
    case STT_NOTYPE:
    	return "NOTYPE";
    case STT_OBJECT:
    	return "OBJECT";
    case STT_FUNC:
    	return "FUNC";
    case STT_SECTION:
    	return "SECTION";
    case STT_FILE:
    	return "FILE";
    case STT_LOPROC:
    	return "STT_LOPROC";
    case STT_HIPROC:
    	return "STT_HIPROC";
    }
    return "Erreur";
}


void print_RelTab_head(int RelSize, unsigned char *sh_name, unsigned int sh_offset){

	printf("Section de réadressage '%s' à l'adresse de décalage 0x%3x contient %d"
		,sh_name
		,sh_offset
		,RelSize);

	if(RelSize==1)
		printf(" entrée:\n");
	else
		printf(" entrées:\n");

	printf(" %-11s %-7s %-15s %-10s %s\n"
	,"Décalage"
	,"Info"
	,"Type"
	,"Val.-sym"
	,"Noms-symboles");

}

void affiche_Relocation(Elf32_Shdr*Sec, Elf32_Ehdr h, Elf32_Sym *Sym, unsigned char *strtab, FILE *file){
	int RelSize;
	int i,j;
	char* type="";
	char *name=malloc(SIZENAME*sizeof(char));
	if(h.e_type!=ET_REL){
		printf("Il n'y a pas de réadressage dans ce fichier.\n");
	}else{
		for(i=0;i<h.e_shnum;i++){
			if(Sec[i].sh_type==SHT_REL){
				
				printf("\n");
				RelSize=Sec[i].sh_size/Sec[i].sh_entsize;

				print_RelTab_head(RelSize, strtab+Sec[i].sh_name, Sec[i].sh_offset);

				Elf32_Rel *REL_tab=read_REL_tab(h, Sec, file, i);


				for(j=0;j<RelSize;j++){
					int indice_name = ELF32_R_SYM(REL_tab[j].r_info);
					int indice_symbol = ELF32_R_SYM(REL_tab[j].r_info);
					
					char * Stype = get_sym_type(ELF32_ST_TYPE(Sym[indice_symbol].st_info));
					
					if(strcmp(Stype,"NOTYPE")==0){
        				name = SymName(Sec, Sym[indice_name].st_name, Sec[indice_symbol].sh_link, file);
					}else if(strcmp(Stype,"SECTION")==0){
        				name = SymName(Sec, Sec[Sym[indice_name].st_shndx].sh_name, h.e_shstrndx, file);
					}
					type=reloc_types(REL_tab[j]);

					printf("%08x  %08x %-18s%08x   %s\n"
					,REL_tab[j].r_offset
					,REL_tab[j].r_info
					,type
					,Sym[ELF32_R_SYM(REL_tab[j].r_info)].st_value
					,name);
									
				}
			}
		}
	}
	
}


char* SymName(Elf32_Shdr *Sec, int index, int indexStrSym, FILE *file){
  char* name = malloc(SIZENAME*sizeof(char));
  fseek(file,Sec[indexStrSym].sh_offset + index, SEEK_SET);
  char *g __attribute__((unused)) = fgets(name, SIZENAME, file);

  return name;
}
