#define MAX_ELEM 100
#include "prototype.h"
#include "string.h"
#include <ctype.h>


void clear_buffer()
{
    int ind;
    //printf("\n taille: %d",taille );
    for(ind=0; ind<=taille; buffer[ind++]='\0');
    taille = -1;
    //printf("\n fin");
}/*kjddjsklfdsjdfs*/

void buffer_char(char c)
{
    buffer[++taille] = c;
}

void lexical_error(char in_char)
{
    extern int numLigne;
    if(in_char==':')
        printf("\n### ERROR A LA LIGNE %d:  %c DEUX POINTS DOIT ETRE SUIVI D'UN EGAL ###",numLigne, in_char);
    else if(in_char == '.')
        printf("\n### ERROR POINT A LA LIGNE %d ###",numLigne);
    else
        printf("\n### ERROR A LA LIGNE %d: UNKNOWN TOKEN ###",numLigne);

}
token check_reserved()
{
    //printf("--- %s", buffer);
    if(strcmp(buffer,"begin") == 0) return begin;
    if(strcmp(buffer,"end") == 0) return end;
    if(strcmp(buffer,"read") == 0) return read;
    if(strcmp(buffer,"write") == 0) return write;
    if(strcmp(buffer,"scaneof") == 0) return scaneof;
    return id;
}
token scanner(FILE *pt_fichier)
{
    int in_char, c;
    extern int numLigne;
    int isFloat = 0;

    clear_buffer();
    if(feof(pt_fichier)) return scaneof;

    if(pt_fichier==NULL)
    {
        printf("Fichier introuvable\n");
        exit(0);
    }

    while((in_char = fgetc(pt_fichier)) != feof)
    {
        if(feof(pt_fichier)) return scaneof;
        //printf("\nCARAC %c",in_char);
        if(isspace(in_char)) continue;

        else if(isalpha(in_char))
        {
            //printf("\nPASS PAR LA alpha");
            buffer_char(in_char);
            for(c=getc(pt_fichier); (isalnum(c) || c=='_') && c != '\n' && c != ';'; c=getc(pt_fichier)){

                //printf("\n------------> %c", c);
                buffer_char(c);
            //printf("\n || BUFFER CONTAIN :: %s ||\n\n\n",buffer );
            }
            if(c=='\n') numLigne++;
            ungetc(c, pt_fichier);
            return check_reserved();
        }
        else if(isdigit(in_char))
        {

            buffer_char(in_char);
            //tq c'est digit => on stock
            for(c=fgetc(pt_fichier); isdigit(c) && c !='\n'; c=getc(pt_fichier)){

                buffer_char(c);
            }
            //il n'est plus entier, (on teste si il est reel)
            if (c == '.'){
                    buffer_char(c);//on ajoute

                    for(c=fgetc(pt_fichier); isdigit(c) && c !='\n'; c=getc(pt_fichier)) buffer_char(c);
                    if(c=='\n') numLigne++;
                    if (c == '.'){

                        for(c=fgetc(pt_fichier);  (c=='.'||isdigit(c) )&& c !='\n'; c=getc(pt_fichier)) buffer_char(c);
                        if(c=='\n') numLigne++;
                        lexical_error('.');
                    }else if(c=='e' || c=='E'){

                        for(c=fgetc(pt_fichier);  (c=='.'|| isdigit(c) || c=='e' || c=='E' || c=='+' || c=='-' )&& c !='\n'; c=getc(pt_fichier))
                        {
                            if (c == '-')   isFloat = 1;
                            if(c=='.') lexical_error(c);
                            else if( c=='e' || c=='E') lexical_error(c);
                            buffer_char(c);
                        }
                    }
                }else if(c=='e' || c=='E'){
                    buffer_char(in_char);
                    for( c=fgetc(pt_fichier) ;  isdigit(c)|| c=='+' || c=='-' ; c=fgetc(pt_fichier) ){
                            if (c == '-')
                                isFloat = 1;
                            buffer_char(c);
                    }
                    ungetc(c, pt_fichier);
                    if (isFloat == 1){
                        isFloat = 0;
                        return floatLet;
                    }
                    return intLet;
                }else{ //sinon il n'est pas réel (on retourne le dernier caractere lu au fichier, et on retourne un int)
                ungetc(c, pt_fichier);
                if (isFloat == 1){
                        isFloat = 0;
                        return floatLet;
                    }
                return intLet;
                }
                ungetc(c, pt_fichier);
                return floatLet;
        }
        else if(in_char == '.')
        {
            buffer_char(in_char);
            c= fgetc(pt_fichier);
            if(isdigit(c))
            {
                for(c=fgetc(pt_fichier); isdigit(c) && c!='\n'; c=getc(pt_fichier)) buffer_char(c);
                if(c=='\n') numLigne++;
                else if(c=='.') lexical_error(c);
                else if(c=='e' || c=='E'){
                    buffer_char(in_char);
                    for( c=fgetc(pt_fichier) ;  isdigit(c) || c=='+' || c=='-' ; c=fgetc(pt_fichier) ) buffer_char(c);
                    ungetc(c, pt_fichier);
                    //printf("lkfkdkd");
                    return floatLet;
                }
                return floatLet;
            }
            else if(c == '.')
            {
                for(c=fgetc(pt_fichier); (c=='.' || isdigit(c)) && c!='\n'; c=getc(pt_fichier)) buffer_char(c);
                   if(c=='\n') numLigne++;
                lexical_error(in_char);
                ungetc(c, pt_fichier);
                return floatLet;
            }
            else if(c == '\n')
            {
                numLigne++;
                ungetc(c,pt_fichier);
                return intLet;
            }
            else if( c=='+' || c=='-' || c=='*' || c=='/' || c=='%')
            {
                ungetc(c,pt_fichier);
                return intLet;
            }
            ungetc(c,pt_fichier);
            lexical_error(in_char);
        }

         if (in_char =='(')
            return lparen;
        else if (in_char == ')')
            return rparen;
        else if (in_char== ';')
        {
            return semicolon;
        }
        else if (in_char == ',')
            return comma;
        else if (in_char == '+')
            return plusop;
        else if (in_char == '-')
            return minusop;
        else if (in_char == ':')
        {
            c = fgetc(pt_fichier);
            if(c == '=')
                return assignop;
            else
            {
                ungetc(c, pt_fichier);
                lexical_error(in_char);
            }
        }
        else if(in_char == '-')
        {
            c = fgetc(pt_fichier);
            if(c == '-')
                {
                    do
                    {
                        in_char = fgetc(pt_fichier);

                    } while(in_char != '\n');

                }
            else
            {
                ungetc(c, pt_fichier);
                return minusop;
            }
        }
        else
        {
            lexical_error(in_char);
        }
        printf("\n****************\n");
    }//fin while
     printf("\nsort du while ****************\n");
}
char* getToken(token t)
{
    char aide[35];
    switch(t)
    {
        case begin   : strcpy(aide, "begin");
        return aide;
        case end     : strcpy(aide, "end");
        return aide;
        case read    :strcpy(aide, "read");
        return aide;
        case write   : strcpy(aide, "write");
        return aide;
        case id      : strcpy(aide, "id");
        return aide;
        case comma   : strcpy(aide, "comma");
        return aide;
        case plusop  : strcpy(aide, "plusop");
        return aide;
        case minusop : strcpy(aide, "minusop");
        return aide;
        case intLet  : strcpy(aide, "intLet");
        return aide;
        case lparen  : strcpy(aide, "lparen");
        return aide;
        case rparen  :  strcpy(aide, "rparen");
        return aide;
        case assignop: strcpy(aide, "assignop");
        return aide;
        case scaneof : strcpy(aide, "scaneof");
        return aide;
        case semicolon : strcpy(aide, "semicolon");
        return aide;
        case floatLet : strcpy(aide, "floatLet");
        return aide;

    }
}
