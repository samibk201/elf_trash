#include "section_header.h"

/////////////////////////////////////////////////////////
//////////////////////// étape 2 ////////////////////////
/////////////////////////////////////////////////////////


int32_t val_32(Elf32_Ehdr h, int32_t value){
	if(h.e_ident[EI_DATA]==ELFDATA2MSB)
		return __bswap_32(value);
	return value;
}


int16_t val_16(Elf32_Ehdr h, int16_t value){
	if(h.e_ident[EI_DATA]==ELFDATA2MSB)
		return __bswap_16(value);
	return value;
}




void read_section(FILE *fichier, Elf32_Shdr *s, Elf32_Ehdr h){
	size_t f __attribute__((unused)) = fread(s,1,sizeof(Elf32_Shdr),fichier);
	
	s->sh_name        = val_32(h, s->sh_name);
	s->sh_type        = val_32(h, s->sh_type);
	s->sh_flags       = val_32(h, s->sh_flags);
	s->sh_addr        = val_32(h, s->sh_addr);
	s->sh_offset      = val_32(h, s->sh_offset);
	s->sh_size        = val_32(h, s->sh_size);
	s->sh_link        = val_32(h, s->sh_link);
	s->sh_info        = val_32(h, s->sh_info);
	s->sh_addralign   = val_32(h, s->sh_addralign);
	s->sh_entsize     = val_32(h, s->sh_entsize);

}


char *read_type (Elf32_Shdr s){
	char *type = "";
	switch(s.sh_type){
		case SHT_NULL:
			type = "NULL";
			break;
		case SHT_PROGBITS:
			type = "PROGBITS";
			break;
		case SHT_SYMTAB:
			type = "SYMTAB";
			break;
		case SHT_STRTAB:
			type = "STRTAB";
			break;
		case SHT_RELA:
			type = "RELA";
			break;
		case SHT_HASH:
			type = "HASH";
			break;
		case SHT_DYNAMIC:
			type = "DYNAMIC";
			break;
		case SHT_NOTE:
			type = "NOTE";
			break;
		case SHT_NOBITS:
			type = "NOBITS";
			break;
		case SHT_REL:
			type = "REL";
			break;
		case SHT_SHLIB:
			type = "SHLIB";
			break;
		case SHT_DYNSYM:
			type = "DYNSYM";
			break;
		case SHT_LOPROC:
			type = "LOPROC";
			break;
		case SHT_HIPROC:
			type = "HIPROC";
			break;
		case SHT_LOUSER:
			type = "LOUSER";
			break;
		case SHT_HIUSER:
			type = "HIUSER";
			break;
		case SHT_ARM_ATTRIBUTES:
			type = "ARM_ATTRIBUTES";
			break;
		default:
			break;
	}
	return type;
}


char *read_flags (char * flags, Elf32_Word flag){	
	
	if (SHF_WRITE & flag)
		strcat(flags, "W");
	if (SHF_ALLOC & flag)
		strcat(flags, "A");
	if (SHF_EXECINSTR & flag)
		strcat(flags, "X");
	if (SHF_INFO_LINK & flag)
		strcat(flags, "I");
	if (SHF_TLS & flag)
		strcat(flags, "T");
	if (SHF_GROUP & flag)
		strcat(flags, "G");
	if (SHF_OS_NONCONFORMING & flag)
		strcat(flags, "O");
	if (SHF_LINK_ORDER & flag)
		strcat(flags, "L");
	if (SHF_MERGE & flag)
		strcat(flags, "M");
	if (SHF_STRINGS & flag)
		strcat(flags, "S");
	return flags;
}

void print_espace (int i,int nb) {
	for (;i <nb; i ++) printf(" ");
}

void print_section (FILE *file, Elf32_Shdr *T, Elf32_Ehdr *e){
	
	Elf32_Shdr S;
	char *name = malloc(SIZENAME*sizeof(char)), *type, *flag;
	int i;
	

	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x:\n\n",e->e_shnum, e->e_shoff);
	printf("En-têtes de section :\n");
	printf("  [Nr] Nom               Type            Adr      Décala.Taille ES Fan LN Inf Al\n");	
	for (i=0; i<e->e_shnum; i++){	 //tant qu'on a des entrées à lire
		S = T[i];

		// Récupération du type 
		type = read_type(S);
		printf("  [%2d] ", i);
		
		//Récupération du flag
		flag = malloc(sizeof(char*));
		flag = read_flags(flag, S.sh_flags);

		// Récupération du nom
		fseek(file, T[e->e_shstrndx].sh_offset+S.sh_name, SEEK_SET);
		char *g __attribute__((unused)) = fgets(name, SIZENAME, file);
		if (strcmp(name,".rel.debug_aranges") == 0) 
			name [strlen(name) - 1] ='\0';
		// Affichage de tous les éléments de la section
		printf("%s", name);print_espace (strlen(name),18);
		printf("%s", type);print_espace (strlen(type),15);
		printf(" %08x", S.sh_addr);
		printf(" %06x", S.sh_offset);
		printf(" %06x", S.sh_size);
		printf(" %02x", S.sh_entsize);
		printf("  %2s",flag);
		free(flag);
		printf(" %2d", S.sh_link);
		printf("  %2d", S.sh_info);
		printf(" %2d", S.sh_addralign);
		printf("\n");
	}
	printf("Clé des fanions :\n");
	printf("  W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes), I (info),\n");
	printf("  L (ordre des liens), O (traitement supplémentaire par l'OS requis), G (groupe),\n");
	printf("  T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),\n");
	printf("  y (purecode), p (processor specific)\n");
	free(name);
}

void get_sh_values(Elf32_Shdr **sectionHeader, FILE* f,Elf32_Ehdr *h){
	fseek(f,h->e_shoff,SEEK_SET);
	int i;
    for(i=0;i<h->e_shnum;i++)
    	read_section(f,&((*sectionHeader)[i]),*h);
}





