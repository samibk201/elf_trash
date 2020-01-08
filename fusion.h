#ifndef _H_FUSION_
#define _H_FUSION_
#define SIZENAME 30
#include <stdio.h>
#include <elf.h>

typedef struct {
    Elf32_Shdr* newsh;		//contient : soit une section , ou deux sections si il y a la meme section dans l autre
    Elf32_Word type;
    Elf32_Word size;                      
	Elf32_Off offset;
	Elf32_Word	sh_link;
    Elf32_Word	sh_info;
    char* name;
	int nbS;
}Donnees_fusion; //contient les section a fusionnees

typedef struct {
	Elf32_Shdr* sh1, * sh2;
	Donnees_fusion *f ;
	int nbS1, nbS2;
	Elf32_Off offset;
	int o_ecris;
} Donnees;

//libère de la mémoire
void freemem(void *adr);

/**Cette fonction initilaisation de des sections de la structure Donnees
 *-Données : deux section,une réfrence sur le nombre de section qui sera modifié,une entête
 * -Résultat : une section initialisée sinon assert gère l'erreur
 * **/
Elf32_Shdr*  init_sections(Elf32_Shdr* sh, int* nbS, Elf32_Shdr* sections_table, Elf32_Ehdr* header);

/**Cette fonction initialise une structure Donnees pour la fusion,
 * Données : une structure Donnees,deux tables de section,deux entêtes *
 * Résultat : Elle ne renvoie rien mais d sera modifiée
 * */
void init_fusion(Donnees* d, Elf32_Shdr* sections_table1, Elf32_Ehdr* header1, Elf32_Shdr* sections_table2, Elf32_Ehdr* header2);

/**Cette fonction récupère le nom d'une section par son indice
 * Données : l'entet elf,la table de section,l'indice idx puis un stream ouvert en lecture
 * -Résultat : un string qui est le nom de la section de l'indice idx
 * **/
char* get_section_name(Elf32_Ehdr* header, Elf32_Shdr* sections_table, int idx, FILE* fichier);

/**Cette fonction vérifie l'existence d'une section donnée par nom dans la table de section du fichier donné
 * Données : le nom de la section,une entete elf,une table de section et un fichier
 * Résultat : -1 si le nom trouvé sinon l'indice au quel ce nom est trouvé
 * **/
int is_in(char * nameS,Elf32_Ehdr* h, Elf32_Shdr* t, FILE* f);

/**Cette fonction fusionne les deux fichiers
 * Si section type existe dans les deux fichiers ou si le type n'existe que dans le premier fichier 
 * Données : une structure Donnees,deux fichiers ouverts en lecture,deux entete elf,et un type de section
 * Résultat : cette fonction ne renvoie rien mais modifie la structure donnée qui contiendra la fusion
 * **/
void fusion_section1_in_section2(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type);
/**Cette fonction fusionne les deux fichiers
 * si une section se trouve uniquement dans le fichier deux on l'ajoute à la fin
 * Données : Une structure Donnees,deux stream ouverts en r,deux entetes elf et type de section
 * Résultat : elle ne renvoie rien mais modifie la structure Données
 * */
void fusion_section2_in_section1(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type);


/**Cette fonction fait une fusion par type en appelant les deux autres fonctions de fusion
 * Données : Une structure Données,deux streams ouverts en "r",deux entêtes elf et un type
 * Résultat : Elle ne renvoie rien mais modifie la structure Données
 *  * */
void fusion_by_type(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type);


/**Cette fonction permet de diviser le nom d'une section de type REL ou RAL aprtir du deuxieme point 
 * Données : Une chaine de caractere 
 * Résultat : Une chaine de caractere 
 *  * */
char* split_name(char * name);

/**Cette fonction permet de retourner l'indice d'une section dans la structure Données a travers son nom
 * Données : Une structure Données, nom de section 
 * Résultat : Elle renvoie un indice de section 
 *  * */
int get_index_section(Donnees * d , char* name);

/**Cette fonction permet la mise a jour  des attributs sh_link et sh_info des sections de la structure Données
 * Données : Une structure  
 * Résultat : Elle ne renvoie rien mais modifie la structure Données
 *  * */
void modification_indx_sections(Donnees * d);

/**Cette fonction permet l'ecriture de l'entete
 * Données : l'entete ELF , un stream ouvert en "w", Une structure  Données 
 * Résultat : Elle ne renvoie rien
 *  * */
void ecriture_entete(Elf32_Ehdr* elf_head, FILE* file, Donnees* d);


/**Cette fonction permet l'ecriture de la table des sections 
 * Données : l'entete ELF , un stream ouvert en "w", la structure  Données
 * Résultat : Elle ne renvoie rien
 *  * */
void ecriture_section_table(Elf32_Ehdr* elf_head, FILE* file, Donnees* d);

#endif
