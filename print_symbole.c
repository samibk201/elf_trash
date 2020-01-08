#include <stdio.h>
#include "print_symbole.h"

void print_type_symb(int info)
{
    switch (ELF32_ST_TYPE(info))
    {
        case STT_NOTYPE: printf("NOTYPE  ");
            break;
        case STT_OBJECT: printf("OBJECT  ");
            break;
        case STT_FUNC: printf("FUNC    ");
            break;
        case STT_SECTION: printf("SECTION ");
            break;
        case STT_FILE: printf("FILE    ");
            break;
        case STT_LOPROC: printf("LOPROC  ");
            break;
        case STT_HIPROC: printf("HIPROC  ");
            break;
        default: printf("NOTYPE  ");          
            break;
    }
}

void print_link(int info)
{
    switch (ELF32_ST_BIND(info))
    {
        case STB_LOCAL: printf("LOCAL  ");
            break;
        case STB_GLOBAL: printf("GLOBAL ");
            break;
        case STB_WEAK: printf("WEAK   ");
            break;
        case STB_LOPROC: printf("LOPROC ");
            break;
        case STB_HIPROC: printf("HIPROC ");
            break;
        default: printf("GLOBAL ");            
            break;
    }
}

void print_visi(int other)
{
    switch (ELF32_ST_VISIBILITY(other))
    {
        case STV_DEFAULT: printf("DEFAULT  ");
            break;
        case STV_INTERNAL: printf("INTERNAL ");
            break;
        case STV_HIDDEN: printf("HIDDEN   ");
            break;
        case STV_PROTECTED: printf("PROTECTED");
            break;
    default: printf("RESERVED ");
        break;
    }
}

