#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <byteswap.h>
#include "print_content_section.h"



								/////////////////////////////////////////////////////////
								//////////////////////// étape 3 ////////////////////////
								/////////////////////////////////////////////////////////



//chercher si la section existe  
int existe_section(unsigned char * sectionHexa, Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name){
	int i;
	if(name[0]==46){
		for(i=0;i< header->e_shnum;i++){
			if(!strcmp(name,(char *)sectionHexa + sectionHeader[i].sh_name)){
				return i;
			}
		}	
	} else {
		return atoi(name);
	}
	return -2;	
}
//affiche le contenu d'un tampon en format hex 
void hexdump(int addr,unsigned char tampon[N],int k){
			int i;
		    printf(" 0x%08x  ",addr);
	        addr+=16;
	        for(i=0;i<N;i++){
	            if(i<k){
	                printf("%02x",tampon[i]);
	            }
	            else{
	                printf("   ");
	            }
			    if((i+1)%4==0){
				printf(" ");
			    } 
			}
	    	for(i=0;i<N;i++){
	            if(i<k){
	      			printf("%c",isprint(tampon[i])?tampon[i]:'.');
	            }
	            else{
	                printf(" ");
	            }
	    	}
	    	printf("\n");
}

//lecture d`une chaine de caractére a partir d`une addresse donnée 
void lire_contentsec(FILE *fichier,int addr,int size){
  unsigned char tampon[N]; 
  int k;
		while(size>sizeof(tampon)){
	    	k=fread(tampon,1,sizeof(tampon),fichier);
	    	assert(k == sizeof(tampon));
	    	hexdump(addr,tampon,k);
	    	size-=16;
	  	}
	  	k=fread(tampon,1,size,fichier);
    	assert(k == size);
	   	hexdump(addr,tampon,k);
}

//fonction global d'affichage du contenu d'une section
void affiche_contentSection(unsigned char * sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *fichier, char* nm){
		int k,n = existe_section(sectionHexa, section, header, nm);
		unsigned char *name = (unsigned char *)sectionHexa + (section[n].sh_name);
		if(section[n].sh_type == SHT_NOBITS || section[n].sh_type == SHT_NULL || section[n].sh_size == 0){
				char* namevide;
				if (strcmp((const char *)name, "0") == 0){
					namevide = "";
					printf("  La section « %s » n'a pas de données à vidanger.\n", namevide);
				}else{
					printf("  La section « %s » n'a pas de données à vidanger.\n", name);
				}
		}else{
			if(n>=0){

				printf("\n");
				printf("  Vidange hexadécimale de la section « %s » : \n",name);			
				int addr= section[n].sh_addr;
				int size= section[n].sh_size;
				k = fseek(fichier, section[n].sh_offset, SEEK_SET);
				assert(k != -1);
				lire_contentsec(fichier,addr,size);
				printf("\n");
			} else {
				printf("readelf: AVERTISSEMENT: La section « %s » n'a pas été vidangée parce qu'inexistante !", name);
			
			}
		}	
}

//lecture d'une section aprtir d'un fichier ELF (+ l`affichage depend du boolean bool) 
unsigned char *print_content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier, int bool){
	Elf32_Shdr table;
	int k;
	//calcule de l'offset du debut du contenu d'une section par rapport a l'entete  
	int offset = entete->e_shoff + entete->e_shstrndx * entete->e_shentsize;
	//se deplacer par le debut du des entetes de section + inde
	k=fseek(fichier, offset , SEEK_SET);
	assert(k != -1);
	//charger la table des strings
	k = fread(&table, sizeof(char), sizeof(Elf32_Shdr), fichier);
    assert(k ==sizeof(Elf32_Shdr));  
    k=fseek(fichier,bswap_32(table.sh_offset), SEEK_SET);
    assert(k != -1);
    //alocations de la table des string
    unsigned char * sectionHex = (unsigned char * )malloc( sizeof(unsigned char) * bswap_32(table.sh_size) );
    assert(sectionHex!=NULL);
    k=fread(sectionHex, sizeof(char),bswap_32(table.sh_size), fichier);
    assert(k == bswap_32(table.sh_size) );	
    if(bool)
    	affiche_contentSection(sectionHex, section, entete, fichier, name);

    return sectionHex;
}
