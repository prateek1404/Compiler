// A simplified Scanner for the given grammar. It takes the character stream as an inputfile.txt and prints out the token stream into outputscanner.txt file.

#include<stdio.h>
#include<stdlib.h>
#include"symtab.h"
#define MAX 1000				// Maximum size of the Buffer to hold the character stream.
#define MAXTLEN 50				// Maximum length of each token.

typedef enum{FALSE,TRUE}boolean;
typedef enum{TK_OPEN,TK_CLOSE,TK_SEMI,TK_COMMA,TK_EQUAL,TK_DOT,TK_PLUS,TK_MINUS,TK_TIMES,TK_DIV,TK_ERROR,TK_LT,TK_GT,TK_LEQ,TK_GEQ,TK_NOTEQUAL,TK_ID,TK_NUM,
		TK_KEY_ENDOFFILE,TK_KEY_BEGIN,TK_KEY_END,TK_KEY_MAIN,TK_KEY_FUN,TK_KEY_RESULT,TK_KEY_NONE,TK_KEY_INT,TK_KEY_IF,TK_KEY_CALL,TK_KEY_WHILE,
		TK_KEY_PRINT,TK_KEY_GET,TK_KEY_RETURN}tokenType;
typedef enum{START,SLESS,SGREAT,SNOT,SNUM,SID,FINAL}stateType;

typedef struct{
char str[MAXTLEN];				// stores the value of the identifier or the number itself.
tokenType tok;					// stores the token type.
int ln;						// stores line no. of token
}token;


extern void initializeScanner(char * filename); // opens the inputfile.txt and reads its contents into a buffer.
extern token nextToken();			// fetches the next token.
extern void printTokenStream(FILE * fp);	// prints out the token stream into outputscanner.txt.
extern boolean hasNext();			// checks if any more tokens are left.
extern void reInitialize(); 			// takes the pointer of buffer back to initial position once scanning is complete.
