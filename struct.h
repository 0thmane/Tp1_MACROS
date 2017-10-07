#define MAX_ELEM 100

typedef enum
{
    id, begin, end, read, write, colomn, comma, plusop, minusop, intLet, lparen, rparen, assignop, scaneof, semicolon,floatLet

}token;

char buffer[MAX_ELEM];
int taille=-1;
int numLigne=1;

