#include<string.h>
#include"scanner.h"

char buffer[MAX];			// Buffer stores the character stream from the input file.
char tokenStr[MAXTLEN];			// Stores the token stream being read and which is to be written to the output file.
int pointer = 0;			// Points to the place from where to read the next character in the buffer.
int lineno = 1;				// Stores line no of the token currently being read 
/* initializeScanner opens the input file and stores its text into the buffer. */

void initializeScanner(char *inputFileName)
{
	FILE *fp1;
	fp1 = fopen(inputFileName,"r");
	int j = 0;
	while(!feof(fp1))
	{
		buffer[j++] = fgetc(fp1);
	}
	fclose(fp1);
}

/* Sets the pointer of the buffer back to the starting position.*/
void reInitialize()
{
	pointer = 0;
}

/* Fetches the next token from the buffer.*/ 
token nextToken()
{
	int index = 0;			// gives position of current character in current token.
	tokenType current;		// gives tokenType of current token.
	stateType state = START;	
	int flag;			// indicates whether to store characterStream in tokenStr. 

	while(state != FINAL)
	{
		char c = buffer[pointer];
		flag = 1;
		switch(state)
		{
			case START :
				if(isdigit(c))
				{
					state = SNUM;
				}
				else if(isalpha(c))
				{
					state = SID;
				}
				else if(c == '<')
				{
					state = SLESS;
				}
				else if(c == '>')
				{
					state = SGREAT;
				}
				else if(c == '!')
				{
					state = SNOT;
				}
				else if((c == ' ') || (c == '\t') || (c == '\n'))
				{
					flag = 0;
					if(c == '\n')
					{
						lineno++;
					}
				}
				else
				{
					state = FINAL;
					switch(c)
					{
						case EOF:
							flag = 0;
							current = TK_KEY_ENDOFFILE;
							break;
						case ';':
							current = TK_SEMI;
							break;
						case '(':
							current = TK_OPEN;
							break;	
						case ')':
							current = TK_CLOSE;
							break;
						case '=':
							current = TK_EQUAL;
							break;
						case '+':
							current = TK_PLUS;	
							break;
						case '-':
							current = TK_MINUS;
							break;
						case '*':
							current = TK_TIMES;
							break;
						case '/':
							current = TK_DIV;
							break;
						case '.':
							current = TK_DOT;
							break;
						case ',':
							current = TK_COMMA;
							break;
						default :
							current = TK_ERROR;
					}
				}
				break;
				
			case SLESS:
				if(c =='=')
				{
					flag = 1;
					current = TK_LEQ;
					state = FINAL;
				}
				else
				{
					pointer--;
					current = TK_LT;
					state = FINAL;
				}
				break;
			case SGREAT:
				if(c =='=')
				{
					flag = 1;
					current = TK_GEQ;
					state = FINAL;
				}
				else
				{
					pointer--;
					current = TK_GT;
					state = FINAL;
				}
				break;
			case SNOT:
				if(c =='=')
				{
					flag = 1;
					current = TK_NOTEQUAL;
					state = FINAL;
				}
				else
				{
					pointer--;
					current = TK_ERROR;
					state = FINAL;
				}
			break;
		
			case SNUM:
				if(isalpha(c))
				{
					pointer--;
					flag = 0;
					state = FINAL;
					current = TK_ERROR;
				}
				else if(isdigit(c))
				{
					flag = 1;
					state = SNUM;;
					current = TK_NUM;
				}
				else
				{
					pointer--;
					flag = 0;
					state = FINAL;
					current = TK_NUM;
				}
			break;
			case SID:
				if(isalpha(c) || isdigit(c))
				{
					flag = 1;
					state = SID;
					current = TK_ID;
				}
				else 
				{
					pointer--;
					flag = 0;
					state = FINAL;
					current = TK_ID;
				}
		}
		
		pointer++;
		if(flag && index <= MAXTLEN)
		{
			tokenStr[index++] = c;
		}
		if(state == FINAL)
		{
			tokenStr[index] = '\0';
			token t1;
			strcpy(t1.str,tokenStr);
			if(!(strcmp(t1.str,"begin") ))
			{
				t1.tok = TK_KEY_BEGIN;
			}
			else if(!(strcmp(t1.str,"end") ))
			{
				t1.tok = TK_KEY_END;
			}
			else if(!(strcmp(t1.str,"main") ))
			{
				t1.tok = TK_KEY_MAIN;
			}
			else if(!(strcmp(t1.str,"function") ))
			{
				t1.tok = TK_KEY_FUN;
			}
			else if(!(strcmp(t1.str,"result") ))
			{
				t1.tok = TK_KEY_RESULT;
			}
			else if(!(strcmp(t1.str,"none") ))
			{
				t1.tok = TK_KEY_NONE;
			}
			else if(!(strcmp(t1.str,"int") ))
			{
				t1.tok = TK_KEY_INT;
			}
			else if(!(strcmp(t1.str,"if") ))
			{
				t1.tok = TK_KEY_IF;
			}
			else if(!(strcmp(t1.str,"call") ))
			{
				t1.tok = TK_KEY_CALL;
			}
			else if(!(strcmp(t1.str,"while") ))
			{
				t1.tok = TK_KEY_WHILE;
			}
			else if(!(strcmp(t1.str,"print") ))
			{
				t1.tok = TK_KEY_PRINT;
			}
			else if(!(strcmp(t1.str,"get") ))
			{
				t1.tok = TK_KEY_GET;
			}
			else if(!(strcmp(t1.str,"return") ))
			{
				t1.tok = TK_KEY_RETURN;
			}
			else
			{
				t1.tok = current;
			}
			t1.ln = lineno;
			return t1;
		}
	}
}

/* checks if any more tokens are yet to come .*/

boolean hasNext()
{
	int a = buffer[pointer];
	while(a == ' ' || a=='\n' || a == '\t')
	{
		a = buffer[++pointer];
	}
	if(a == EOF)				
	{
		return FALSE;
	}
	return TRUE;
}


/* prints out the token stream generated to the output file.*/

void printTokenStream(FILE *fp2)
{	
	while(hasNext())
	{
		token t = nextToken();
		
		if(t.tok == TK_KEY_ENDOFFILE)
		{
			fprintf(fp2,"TK_KEY_ENDOFFILE\n");		
		}
		else if(t.tok == TK_SEMI)
		{
			fprintf(fp2,"TK_SEMI\n");
		}

		else if(t.tok == TK_OPEN)
		{
			fprintf(fp2,"TK_OPEN\n");
		}

		else if(t.tok == TK_CLOSE)
		{
			fprintf(fp2,"TK_CLOSE\n");
		}
		else if(t.tok == TK_EQUAL)
		{
			fprintf(fp2,"TK_EQUAL\n");
		}
		else if(t.tok == TK_NOTEQUAL)
		{
			fprintf(fp2,"TK_NOTEQUAL\n");
		}
		else if(t.tok == TK_PLUS)
		{
			fprintf(fp2,"TK_PLUS\n");
		}
		else if(t.tok == TK_MINUS)
		{
			fprintf(fp2,"TK_MINUS\n");
		}
		else if(t.tok == TK_TIMES)
		{
			fprintf(fp2,"TK_TIMES\n");
		}
		else if(t.tok == TK_DIV)
		{
			fprintf(fp2,"TK_DIV\n");
		}
		else if(t.tok == TK_LT)
		{
			fprintf(fp2,"TK_LT\n");
		}
		else if(t.tok == TK_GT)
		{
			fprintf(fp2,"TK_GT\n");
		}
		else if(t.tok == TK_LEQ)
		{
			fprintf(fp2,"TK_LEQ\n");
		}
		else if(t.tok == TK_GEQ)
		{
			fprintf(fp2,"TK_GEQ\n");
		}
		else if(t.tok == TK_DOT)
		{
			fprintf(fp2,"TK_DOT\n");
		}
		else if(t.tok == TK_COMMA)
		{
			fprintf(fp2,"TK_COMMA\n");
		}
		
		else if(t.tok == TK_ID)
		{
			fprintf(fp2,"TK_ID (%s) \n",t.str);
		}
		else if(t.tok == TK_KEY_BEGIN)
		{
			fprintf(fp2,"TK_KEY_BEGIN\n");
		}
		else if(t.tok == TK_KEY_END)
		{
			fprintf(fp2,"TK_KEY_END\n");
		}
		else if(t.tok == TK_KEY_MAIN)
		{
			fprintf(fp2,"TK_KEY_MAIN\n");
		}
		else if(t.tok == TK_KEY_FUN)
		{
			fprintf(fp2,"TK_KEY_FUN\n");
		}
		else if(t.tok == TK_KEY_RESULT)
		{
			fprintf(fp2,"TK_KEY_RESULT\n");
		}
		else if(t.tok == TK_KEY_NONE)
		{
			fprintf(fp2,"TK_KEY_NONE\n");
		}
		else if(t.tok == TK_KEY_INT)
		{
			fprintf(fp2,"TK_KEY_INT\n");
		}
		else if(t.tok == TK_KEY_IF)
		{
			fprintf(fp2,"TK_KEY_IF\n");
		}
		else if(t.tok == TK_KEY_CALL)
		{
			fprintf(fp2,"TK_KEY_CALL\n");
		}
		else if(t.tok == TK_KEY_WHILE)
		{
			fprintf(fp2,"TK_KEY_WHILE\n");
		}
		else if(t.tok == TK_KEY_PRINT)
		{
			fprintf(fp2,"TK_KEY_PRINT\n");
		}
		else if(t.tok == TK_KEY_GET)
		{
			fprintf(fp2,"TK_KEY_GET\n");
		}
		else if(t.tok == TK_KEY_RETURN)
		{
			fprintf(fp2,"TK_KEY_RETURN\n");
		}
		else if(t.tok == TK_NUM)
		{
			fprintf(fp2,"TK_NUM (%s) \n",t.str);
		}
		else if(t.tok == TK_ERROR)
		{
			fprintf(fp2,"TK_ERROR in line no %d\n",lineno);
		}
	}
	fclose(fp2);
}








		
