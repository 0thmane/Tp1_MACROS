#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"

int main()
{
    char resultat[MAX_ELEM];
    resultat[0]='\0';
    char in_char;
    token t;
    int ind;
    char aide[35];
    FILE *pt_fichier = fopen("file.txt","r");
    FILE *pt_fichier_sortie = fopen("file_sortie.txt","w+");
    if(feof(pt_fichier))
    {
        return scaneof;
    }
    if(pt_fichier==NULL)
    {
        printf("Fichier introuvable\n");
        exit(0);
    }
      while((in_char = getc(pt_fichier)) != scaneof)
    {
        ungetc(in_char, pt_fichier);
        t = scanner(pt_fichier);
        if(t==scaneof) break;
        strcpy(aide,getToken(t));
        strncat(aide," ",1);
        strcat(resultat,aide);
    }
        fprintf(pt_fichier_sortie,"%s", resultat);
        printf("\n\n\n#### RESULTAT:   %s ###\n\n\n|  ",resultat);
       // printf("\n\nBUFFER CHAR %s", buffer);
    return 0;
}
