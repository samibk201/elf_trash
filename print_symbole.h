#ifndef __PRINT_SYMBOLE__
#define __PRINT_SYMBOLE__
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

/*ce module gère l'affiche de la table des symboles */

void print_type_symb(int info);

void print_link(int info);

void print_visi(int);

#endif

