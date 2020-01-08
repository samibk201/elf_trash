#ifndef _H_PHEAD_H
#define _H_PHEAD_H

/*Ce module genre l'affichage de l'entete du fichier elf */
void print_e_ident(Elf32_Ehdr *entete);
void print_type(Elf32_Ehdr *entete);
void print_classe_type(Elf32_Ehdr *entete);
void print_codage_type(Elf32_Ehdr *entete);
void print_v_machine(Elf32_Ehdr *entete);
void print_OS(Elf32_Ehdr *entete);
void print_e_version(Elf32_Ehdr *entete);
void print_version (Elf32_Ehdr *entete);
void print_flags (Elf32_Ehdr *entete);
/***Cette fonction fait appel à toutes ses fonctions précendente
	Données : une structure d'entete elf
	Résultat: Aucun
***/
void print_header(Elf32_Ehdr *entete);

#endif
