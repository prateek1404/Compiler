#include"parser.h"
#include<string.h>
char * errormsg;						// Stores the error msg to be printed.
token t;
tree tr;
Link l;
void insertStringName(char *name);
void lookUp(char * name);
hashstruct h(char * name);

ST st;
char *encfun = NULL;
int commacnt = 0;
int begincnt = 0;
boolean retexp = FALSE;
int ms=0;
boolean msexp=FALSE;
boolean callexp=FALSE;
int rt=0;
int hash(char *);
char * callfun = NULL;

void initialiseSymTable()
{	
	int i;
	for(i=0;i<MAXST;i++)
	{
		st[i].s = STU;	
		st[i].name = NULL;
		st[i].encfun = NULL;	
	}
	encfun=(char *)malloc(15*sizeof(char));
	strcpy(encfun,"main");
	callfun=(char *)malloc(15*sizeof(char));
}


void lookUp(char * name)//return and call to be checked
{	
	hashstruct k,j;
	k=h(name);
	if(begincnt == 0)
	{
		if(strcmp(l->tokenstr,"TK_KEY_INT")==0)//similar check for none outside this function
		{
			if(k.flag)
			{
				
				if(st[ms].e.stf->par[commacnt]==1)
				{
				}
				else
				{
					printf("ERROR Type mismatch in Declaration and Defination of %s in line number %d\n",encfun,t.ln);
					exit(1);
					//error...type mismatch
				}
				insertStringName(name);
			}	
			else
			{
				printf("ERROR Token %s redeclared in line number %d in function %s\n",t.str,t.ln,encfun);
				exit(1);
				//error....redeclaration
			}
			commacnt++;
		}
		else if(strcmp(l->tokenstr,"TK_KEY_FUN")==0)
		{
			strcpy(encfun,name);
			if(k.flag)
			{
				printf("ERROR Function %s undeclared, used without declaration in line no %d\n",name,t.ln);
				exit(1);
				//error ...undeclared function
			}
			else
			{
				ms=k.loc;
				if(st[ms].e.stf->dec==1)
				{
					printf("ERROR Redefination of function %s in line no %d\n",encfun,t.ln);
					exit(0);
				}
				st[ms].e.stf->dec=1;
			}
			
		}
		else
		{
			//error garbage...sytax error...redundant
		}
		return;
	}
	k=h(name);
	if(k.flag)
	{
		insertStringName(name);
	}	
	else
	{
		if(strcmp(l->tokenstr,"TK_KEY_INT")==0)
		{
			printf("ERROR token %s redeclared in line no %d in function %s\n",t.str,t.ln,encfun);
			exit(1);
			//error.....redeclaration
		}
		else if(strcmp(l->tokenstr,"TK_KEY_FUN")==0)
		{	
			
			printf("ERROR Function %s redeclared in line no %d\n",name,t.ln);
			exit(0);
			//error.....redeclaration
		}
		else if(strcmp(l->tokenstr,"TK_KEY_RETURN")==0)
		{
			rt=0;
		}
		else if(k.r==1)
		{
			callexp=TRUE;
			strcpy(callfun,name);
			if(st[k.loc].e.stf->ret==0)
			{
				if((strcmp(l->par->par->par->tokenstr,"Statement")==0))
				{
					//correct
				}
				else
				{
					printf("ERROR return type of funtion %s should be none in line no %d\n",callfun,t.ln);
					exit(1);
				}
			}
			else
			{
				if((strcmp(l->par->par->par->tokenstr,"Statement")==0))
				{
					printf("ERROR return type of funtion %s should be int in line no %d\n",callfun,t.ln);
					exit(1);
				}
				else
				{
					//correst
				}
			}
		}
		else
		{
			
			//correct
		}
		
	}

}

hashstruct h(char * name)
{
	hashstruct h={0,0,0};
	int k;
	k=hash(name);
	while(1)
	{
		if(st[k].s == 2)	
		{
			h.loc = k;
			h.flag = 1;
			break;
		}
		else if(st[k].s == 1)
		{	
			if(begincnt==0)
			{
				if(strcmp(st[k].name,name) == 0)
				{
					h.loc = k;
					h.flag = 0;
					break;
				}
				else
				{
					k=(k+1)%MAXST;
				}
			}
			else
			{	
				if(strcmp(encfun,name)==0)
				{
					printf("ERROR Recursion of function %s not allowed in line no %d\n",encfun,t.ln);
					exit(0);
				}
				if(strcmp(st[k].name,name) == 0 && strcmp(st[k].encfun,encfun)==0)
				{
					if(strcmp(encfun,name)==0)
					{
						printf("ERROR Recursion of function %s not allowed in line no %d\n",encfun,t.ln);
						exit(0);
					}
					h.loc = k;
					h.flag = 0;
					h.r=1;
					break;
				}
				else
				{
					k=(k+1)%MAXST;
				}
			}
		}
		else
		{	
			if(strcmp(st[k].name,name) == 0&& strcmp(st[k].encfun,encfun)==0)
			{
				h.loc = k;
				h.flag = 0;
				break;
			}
			else
			{	
				k=(k+1)%MAXST;
			}
	
		}
	}
	return h;
}

void insertStringName(char *name)
{	
	hashstruct k,j;
	k=h(name);
	if(strcmp(l->tokenstr,"TK_KEY_FUN")==0)
	{
		st[k.loc].s = STF;
		st[k.loc].e.stf =(symtabfun *)malloc(sizeof(symtabfun));
		st[k.loc].name = (char *)malloc(15*sizeof(char));
		strcpy(st[k.loc].name,t.str);
		st[k.loc].encfun = (char *)malloc(15*sizeof(char));
		strcpy(st[k.loc].encfun,encfun);
		st[k.loc].e.stf->numarg=0;
		st[k.loc].e.stf->dec=0;
		st[k.loc].e.stf->ret=0;
		st[k.loc].e.stf->par=(type *)malloc(MAXPAR*sizeof(type));
		msexp=TRUE;		///ms inserted afterwards..	
		ms=k.loc;	
	}
	else if(strcmp(l->tokenstr,"TK_KEY_INT")==0)
	{	
		st[k.loc].s = STV;
		st[k.loc].e.stv =(symtabvar *)malloc(sizeof(symtabvar));
		st[k.loc].name = (char *)malloc(15*sizeof(char));
		st[k.loc].encfun = (char *)malloc(15*sizeof(char));
		strcpy(st[k.loc].name,t.str);
		strcpy(st[k.loc].encfun,encfun);
		st[k.loc].e.stv->t=TINT;
		st[k.loc].e.stv->width=4;
		st[k.loc].e.stv->lineno=t.ln;			
	}
	else
	{	
		printf("ERROR %s symbol used for the first time in line no %d\n",t.str,t.ln);
		exit(1);
		//error....used for the first time ....error
	}
}


void createTree()
{
	tr.cur = (Link)malloc(sizeof(Node));
	tr.root = (Link)malloc(sizeof(Node));
	tr.root->child = NULL;
	tr.root->next = NULL;
	tr.root->left = NULL;
	tr.root->par = NULL;
	tr.cur = tr.root;
	tr.cur->tokenstr = (char*)malloc(sizeof(char)*10);
	tr.cur->flag = TRUE;
	strcpy(tr.cur->tokenstr,"RootNode");
}

void etrans()
{
	tr = insertNode(tr,"e",FALSE,t.ln);
}

void eat(tokenType token1)
{
	if(t.tok == token1)
		advance();
	else
	{
		errormsg = strcat(getTokenName(token1)," expected");
		error();
	}
}

void error()
{
	printf("ERROR %s in line no %d \n",errormsg,t.ln);
	exit(1);
}

void advance()
{	
	tr = insertNode(tr,getTokenName(t.tok),FALSE,t.ln);
	t = nextToken();
	if(t.tok == TK_KEY_BEGIN)
	{
		begincnt++;
	}
	else if(t.tok == TK_NUM)
	{
		if(strcmp(l->tokenstr,"TK_KEY_RETURN")==0)
		{
			rt=0;
		}

	}
	else if(t.tok == TK_KEY_RETURN && rt ==0)
	{
		printf("ERROR %s function cant have return in line no %d\n",encfun,t.ln);
		exit(0);
	}
	else if(t.tok == TK_KEY_END)
	{
		begincnt--;
		if(begincnt==0)
		{
			if (rt==0 ||strcmp(encfun,"main")==0)
			{
				//ok
			} 
			else
			{
				printf("ERROR return expected in %s in line number %d\n",encfun,t.ln);
				exit(1);
				//error return expected
			}
		}
	}
	else if(t.tok==TK_ID)
	{
		lookUp(t.str);
	}
	else if(begincnt == 0 && t.tok==TK_CLOSE)
	{
		if (st[ms].e.stf->numarg!=commacnt)
		{
			printf("ERROR Parameter mismatch in function declaration of %s in line number %d\n",encfun,t.ln);
			exit(1);
		}
		commacnt=0;
	}
	else if(msexp)
	{
		if(t.tok==TK_KEY_INT)
		{
			(st[ms].e.stf->numarg)++;
			st[ms].e.stf->par[commacnt]=1;
			commacnt++;
		}
		else if(t.tok==TK_CLOSE)
		{
			commacnt=0;
			msexp=0;
		}
		
	}
	else if(strcmp(l->tokenstr,"TK_KEY_RESULT")==0)
	{	
		if(t.tok==TK_KEY_INT)
		{
			if(begincnt!=0)
			{
				st[ms].e.stf->ret=1;
			}
			else 
			{
				if(st[ms].e.stf->ret !=1)
				{
					printf("Wrong return type in function defination of %s in line number %d\n",encfun,t.ln);
					exit(1);
					//error ...wrong return type
				}
				else
				{
					rt=1;
				}
			}
		}
		else
		{
			if(begincnt==0) 
			{
				if(st[ms].e.stf->ret !=0)
				{
					printf("ERROR wrong return type in function defination of %s in line number %d\n",encfun,t.ln);
					exit(1);
					//error ...wrong return type
				}
				else
				{
					rt=0;
				}
			}
			//write case for none
		}
	}
	else if(callexp)
	{
		if(t.tok==TK_COMMA)
		{
			commacnt++;
		}
		else if(t.tok ==TK_CLOSE)
		{
			commacnt = commacnt+1;
			if(commacnt!=st[ms].e.stf->numarg)
			{
				printf("ERROR Parameter mismatch in function call of %s in line number %d\n",callfun,t.ln);
				//error...parameter mismatch
			}
			commacnt=0;
			callexp=FALSE;
		}
		
	}
}

void printSymTab(char *a)
{
	FILE *f;
	f=fopen(a,"w");
	int i;
	for(i = 0;i < MAXST;i++)
	{	
		if(st[i].s==2)
		{
			continue;
		}
		else if(st[i].s==0)
		{
			fprintf(f,"Name: %s\t\tEncfun: %s\t\tFun or Var:%d\t\tVarType: %d\t\tLine no: %d\t\tWidth: %d\n\n",st[i].name,st[i].encfun,st[i].s,st[i].e.stv->t,st[i].e.stv->lineno,st[i].e.stv->width);
		}
		else if(st[i].s == 1)
		{
			fprintf(f,"Name: %s\t\tEncfun: %s\t\tFun or Var: %d\t\tReturn type: %d\t\tNumArg: %d\n\n",st[i].name,st[i].encfun,st[i].s,st[i].e.stf->ret,st[i].e.stf->numarg);
		}
		else
		{
			
		}
	}
	fclose(f);
}

int hash(char * a)
{
	int i,h=0,j;
	j=strlen(a);
	for(i=0;i<j;i++)
	{
		h=h+a[i]*7;		
	}
	return h%MAXST;
}

tree Parse()
{
	t = nextToken();//only when token is TK_ID
	Programs();
	return tr;
}

// Takes tokentype as input and returns the respective token as String.
char* getTokenName(tokenType tt)
{
	char *p = (char *)malloc(sizeof(char)*25);
	if(tt == TK_OPEN)
		strcpy(p,"TK_OPEN");
	else if(tt == TK_CLOSE)
		strcpy(p,"TK_CLOSE");
	else if(tt == TK_SEMI)
		strcpy(p,"TK_SEMI");
	else if(tt == TK_COMMA)
		strcpy(p,"TK_COMMA");
	else if(tt == TK_EQUAL)
		strcpy(p,"TK_EQUAL");
	else if(tt == TK_DOT)
		strcpy(p,"TK_DOT");
	else if(tt == TK_LT)
		strcpy(p,"TK_LT");
	else if(tt == TK_LEQ)
		strcpy(p,"TK_LEQ");
	else if(tt == TK_GT)
		strcpy(p,"TK_GT");
	else if(tt == TK_GEQ)
		strcpy(p,"TK_GEQ");
	else if(tt == TK_EQUAL)
		strcpy(p,"TK_EQUAL");
	else if(tt == TK_NOTEQUAL)
		strcpy(p,"TK_NOTEQUAL");
	else if(tt == TK_PLUS)
		strcpy(p,"TK_PLUS");
	else if(tt == TK_MINUS)
		strcpy(p,"TK_MINUS");
	else if(tt == TK_TIMES)
		strcpy(p,"TK_TIMES");
	else if(tt == TK_DIV)
		strcpy(p,"TK_DIV");
	else if(tt == TK_NUM)
		strcpy(p,"TK_NUM");
	else if(tt == TK_ID)
		strcpy(p,"TK_ID");
	else if(tt == TK_KEY_ENDOFFILE)
		strcpy(p,"TK_KEY_ENDOFFILE");
	else if(tt == TK_ERROR)
		strcpy(p,"TK_ERROR");
	else if(tt == TK_KEY_BEGIN)
		strcpy(p,"TK_KEY_BEGIN");
	else if(tt == TK_KEY_END)
		strcpy(p,"TK_KEY_END");
	else if(tt == TK_KEY_MAIN)
		strcpy(p,"TK_KEY_MAIN");
	else if(tt == TK_KEY_FUN)
		strcpy(p,"TK_KEY_FUN");
	else if(tt == TK_KEY_RESULT)
		strcpy(p,"TK_KEY_RESULT");
	else if(tt == TK_KEY_NONE)
		strcpy(p,"TK_KEY_NONE");
	else if(tt == TK_KEY_CALL)
		strcpy(p,"TK_KEY_CALL");
	else if(tt == TK_KEY_INT)
		strcpy(p,"TK_KEY_INT");
	else if(tt == TK_KEY_GET)
		strcpy(p,"TK_KEY_GET");
	else if(tt == TK_KEY_RETURN)
		strcpy(p,"TK_KEY_RETURN");
	else if(tt == TK_KEY_PRINT)
		strcpy(p,"TK_KEY_PRINT");
	else if(tt == TK_KEY_WHILE)
		strcpy(p,"TK_KEY_WHILE");
	else if(tt == TK_KEY_IF)
		strcpy(p,"TK_KEY_IF");
	return p;
}

void Programs()
{	
	tr = insertNode(tr,"Programs",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_MAIN:
			Functions();
		 	break;
		default :errormsg = "main keyword expected";
		error();
		break;
	}
	tr = parent(tr);
}

void Functions()
{
	tr = insertNode(tr,"Functions",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_MAIN:
			MainFun();
			OtherFuns();
		 	break;
		default :errormsg = "main keyword expected";
			error();
		break;
	}
	tr = parent(tr);

}

void MainFun()
{
	tr = insertNode(tr,"MainFun",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_MAIN:
			eat(TK_KEY_MAIN);
			eat(TK_KEY_BEGIN);
			FunBody();
			eat(TK_KEY_END);
		 	break;
		default :errormsg = "main keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void OtherFuns()
{
	tr = insertNode(tr,"OtherFuns",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_FUN:
			Funct();
			OtherFuns();
		 	break;
		case TK_KEY_ENDOFFILE:
			etrans();
			break;
		default :errormsg = "fun or eof keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void FunBody()
{
	tr = insertNode(tr,"FunBody",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_END:
		case TK_KEY_FUN:
		case TK_KEY_INT:
		case TK_KEY_IF:
		case TK_KEY_CALL:
		case TK_KEY_WHILE:
		case TK_KEY_PRINT:
		case TK_KEY_GET:
		case TK_KEY_RETURN:
		case TK_ID:
			Declarations();
			OtherStatements();
		 	break;
		default :errormsg = "end or function or int or if or call or while or print or get or return keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Funct()
{
	tr = insertNode(tr,"Funct",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_FUN:
			eat(TK_KEY_FUN);
			eat(TK_ID);
			eat(TK_KEY_RESULT);
			ResultType();
			eat(TK_OPEN);
			Parameters();
			eat(TK_CLOSE);
			eat(TK_KEY_BEGIN);
			FunBody();
			eat(TK_KEY_END);
		 	break;
		default :errormsg = "fun keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ResultType()
{
	tr = insertNode(tr,"ResultType",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_NONE:
			eat(TK_KEY_NONE);
		 	break;
		case TK_KEY_INT:
			eat(TK_KEY_INT);
			break;
		default :errormsg = "none or int keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Parameters()
{
	tr = insertNode(tr,"Parameters",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_NONE:
			eat(TK_KEY_NONE);
		 	break;
		case TK_KEY_INT:
			Parameter();
			RemainingParameters();
			break;
		default :errormsg = "none or int keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Parameter()
{
	tr = insertNode(tr,"Parameter",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_INT:
			eat(TK_KEY_INT);
			eat(TK_ID);
		 	break;
		default :errormsg = "int keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void RemainingParameters()
{
	tr = insertNode(tr,"RemainingParameters",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_CLOSE:
			etrans();
			break;
		case TK_COMMA:
			eat(TK_COMMA);
			Parameter();
			RemainingParameters();
			break;
		default :errormsg = "close or comma keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Declarations()
{
	tr = insertNode(tr,"Declarations",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_FUN:
		case TK_KEY_INT:
			Declaration();
			Declarations();
		 	break;
		case TK_KEY_END:
		case TK_KEY_IF:
		case TK_KEY_CALL:
		case TK_KEY_RETURN:
		case TK_KEY_WHILE:
		case TK_KEY_PRINT:
		case TK_KEY_GET:
		case TK_ID:
			etrans();
			break;
		default :errormsg = "fun or int or if or call or while or print or get or id keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void OtherStatements()
{
	tr = insertNode(tr,"OtherStatements",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_END:
			etrans();
		 	break;
		case TK_KEY_IF:
		case TK_KEY_CALL:
		case TK_KEY_WHILE:
		case TK_KEY_PRINT:
		case TK_KEY_GET:
		case TK_KEY_RETURN:
		case TK_ID:
			Statement();
			OtherStatements();
			break;
		default :errormsg = "end or if or call or while or get or return or id keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Declaration()
{
	tr = insertNode(tr,"Declaration",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_FUN:
			eat(TK_KEY_FUN);
			eat(TK_ID);
			eat(TK_OPEN);
			FunDeclTypes();
			eat(TK_CLOSE);
			eat(TK_KEY_RESULT);
			ResultType();
			eat(TK_SEMI);
		 	break;
		case TK_KEY_INT:
			eat(TK_KEY_INT);
			eat(TK_ID);	
			eat(TK_SEMI);
			break;
		default :errormsg = "function or int keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void FunDeclTypes()
{
	tr = insertNode(tr,"FunDeclTypes",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_NONE:
			eat(TK_KEY_NONE);
		 	break;
		case TK_KEY_INT:
			eat(TK_KEY_INT);
			FunDeclTypesRest();
			break;
		default :errormsg = "none or int keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void FunDeclTypesRest()
{
	tr = insertNode(tr,"FunDeclTypeRest",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_CLOSE:
			etrans();
		 	break;
		case TK_COMMA:
			eat(TK_COMMA);
			eat(TK_KEY_INT);
			FunDeclTypesRest();
			break;
		default :errormsg = "close or comma keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Statement()
{
	tr = insertNode(tr,"Statement",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_IF:
			ConditionalStmt();
		 	break;
		case TK_KEY_CALL:
			FunctionCallStmt();
			break;
		case TK_KEY_WHILE:
			RepetitiveStmt();
			break;
		case TK_KEY_PRINT:
		case TK_KEY_GET:
			IO_Stmt();
			break;
		case TK_KEY_RETURN:
			ReturnStmt();
			break;
		case TK_ID:
			AssignmentStmt();
			break;
		default :errormsg = "if or call or while or get or print or return or id keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void AssignmentStmt()
{
	tr = insertNode(tr,"AssignmentStmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_ID:
			eat(TK_ID);
			eat(TK_EQUAL);
			Expression();
			eat(TK_SEMI);
		 	break;
		default :errormsg = "id keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ConditionalStmt()
{
	tr = insertNode(tr,"ConditionalStmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_IF:
			eat(TK_KEY_IF);
			eat(TK_OPEN);
			BoolExp();
			eat(TK_CLOSE);
			eat(TK_KEY_BEGIN);
			OtherStatements();
			eat(TK_KEY_END);
		 	break;
		default :errormsg = "if keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void RepetitiveStmt()
{
	tr = insertNode(tr,"RepetitiveStmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_WHILE:
			eat(TK_KEY_WHILE);
			eat(TK_OPEN);
			BoolExp();
			eat(TK_CLOSE);
			eat(TK_KEY_BEGIN);
			OtherStatements();
			eat(TK_KEY_END);
		 	break;
		default :errormsg = "while keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ReturnStmt()
{
	tr = insertNode(tr,"ReturnStmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_RETURN:
			eat(TK_KEY_RETURN);
			ReturnVal();
			eat(TK_SEMI);
		 	break;
		default :errormsg = "return keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void IO_Stmt()
{
	tr = insertNode(tr,"IO_Stmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_PRINT:
			eat(TK_KEY_PRINT);
			eat(TK_ID);
			eat(TK_SEMI);
		 	break;
		case TK_KEY_GET:
			eat(TK_KEY_GET);
			eat(TK_ID);
			eat(TK_SEMI);
			break;
		default :errormsg = "print or get keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void FunctionCallStmt()
{
	tr = insertNode(tr,"FunctionCallStmt",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_CALL:
			FunctionCall();
			eat(TK_SEMI);
			break;
		default :errormsg = "call keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Expression()
{
	tr = insertNode(tr,"Expression",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_CALL:
		case TK_ID:
		case TK_NUM:
		case TK_OPEN:
			Term();
			EPrime();
		 	break;
		default :errormsg = " call or id or num or open keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Term()
{
	tr = insertNode(tr,"Term",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_CALL:
		case TK_ID:
		case TK_NUM:
		case TK_OPEN:
			Factor();
			TPrime();
		 	break;
		default :errormsg = "call or id or num or open keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void EPrime()
{
	tr = insertNode(tr,"EPrime",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_PLUS:
			eat(TK_PLUS);
			Term();
			EPrime();
		 	break;
		case TK_MINUS:
			eat(TK_MINUS);
			Term();
			EPrime();
			break;
		case TK_SEMI:
		case TK_CLOSE:
			etrans();
			break;
		default :errormsg = "plus or minus or semi or close keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void Factor()
{
	tr = insertNode(tr,"Factor",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_OPEN:
			eat(TK_OPEN);
			Expression();
			eat(TK_CLOSE);
		 	break;
		case TK_NUM:
			eat(TK_NUM);
			break;
		case TK_ID:
			eat(TK_ID);
			break;
		case TK_KEY_CALL:
			FunctionCall();
			break;
		default :errormsg = "open or num or id or call keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void TPrime()
{
	tr = insertNode(tr,"TPrime",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_TIMES:
			eat(TK_TIMES);
			Factor();
			TPrime();
		 	break;
		case TK_DIV:
			eat(TK_DIV);
			Factor();
			TPrime();
			break;
		case TK_PLUS:
		case TK_MINUS:
		case TK_SEMI:
		case TK_CLOSE:
			etrans();
			break;
		default :errormsg = "times or div or plus or minus keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void FunctionCall()
{
	tr = insertNode(tr,"FunctionCall",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_KEY_CALL:
			eat(TK_KEY_CALL);
			eat(TK_ID);
			eat(TK_OPEN);
			ActualParams();
			eat(TK_CLOSE);
		 	break;
		default :errormsg = "call keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ActualParams()
{
	tr = insertNode(tr,"ActualParams",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_ID:
			eat(TK_ID);
			ActualParamsRest();
		 	break;
		case TK_NUM:
			eat(TK_NUM);
			ActualParamsRest();
			break;
		case TK_KEY_NONE:
			eat(TK_KEY_NONE);
			break;
		default :errormsg = "id or num or none keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ActualParamsRest()
{
	tr = insertNode(tr,"ActualParamsRest",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_CLOSE:
			etrans();
		 	break;
		case TK_COMMA:
			eat(TK_COMMA);
			APrime();
			break;
		default :errormsg = "comma or close keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void BoolExp()
{
	tr = insertNode(tr,"BoolExp",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_ID:
			eat(TK_ID);
			eat(TK_DOT);
			BoolOps();
			eat(TK_DOT);
			eat(TK_ID);
		 	break;
		default :errormsg = "id keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void BoolOps()
{
	tr = insertNode(tr,"BoolOps",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_EQUAL:
			eat(TK_EQUAL);
		 	break;
		case TK_NOTEQUAL:
			eat(TK_NOTEQUAL);
			break;
		case TK_LT:
			eat(TK_LT);
			break;
		case TK_GT:
			eat(TK_GT);
			break;
		case TK_LEQ:
			eat(TK_LEQ);
			break;
		case TK_GEQ:
			eat(TK_GEQ);
			break;
		default :errormsg = "equal or notequal or < or > or <= or >= keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void ReturnVal()
{	
	tr = insertNode(tr,"ReturnVal",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_ID:
			eat(TK_ID);
		 	break;
		case TK_NUM:
			eat(TK_NUM);
			break;
		default :errormsg = "id or num keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

void APrime()
{
	tr = insertNode(tr,"APrime",TRUE,t.ln);
	switch(t.tok)
	{
		case TK_ID:
			eat(TK_ID);
			ActualParamsRest();
		 	break;
		case TK_NUM:
			eat(TK_NUM);
			ActualParamsRest();
			break;
		default :errormsg = "id or num keyword expected";
			error();
		break;
	}
	tr = parent(tr);
}

// Inserts a node in the parse tree.
tree insertNode(tree tree1,char *s,boolean flag1,int line)
{	
	Link link = (Link)malloc(sizeof(Node));
	link->flag = flag1;
	link->lineno=line;
	if(flag1 == TRUE)
	{
		link->tokenstr = (char*) malloc(sizeof(char)*(strlen(s)));
		link->value = NULL;
		link->tokenstr = s;
	}
	else
	{
		if((strcmp(s,"TK_NUM")==0)||(strcmp(s,"TK_ID")==0))
		{
			link->tokenstr = malloc(sizeof(char)*(strlen(s)));
			link->tokenstr = s;
			link->value = (char*)malloc(sizeof(char)*strlen(t.str));
			strcpy(link->value,t.str);
		}
		else
		{
			link->tokenstr = malloc(sizeof(char)*(strlen(s)));
			link->tokenstr = s;
			link->value = NULL;
		}
	}	  
  	l=link;
	if(tree1.cur!=NULL)
	{
		if(tree1.cur->child == NULL)
		{
			tree1.cur->child = link;
			link->next = NULL;
			link->left = NULL;
			link->par = tree1.cur;
			link->child = NULL;
			if(flag1 == TRUE)
			{
			  tree1.cur = link;
			}
		}	
		else
		{	
			Link temp = (Link)malloc(sizeof(Node));
			temp = tree1.cur;
			tree1.cur = tree1.cur->child;
			while(tree1.cur->next != NULL)
			{
				tree1.cur = tree1.cur->next;
			}
			tree1.cur->next = link;
			link->next = NULL;
			link->left = NULL;
			link->par = temp;
			link->child = NULL;
			if (flag1 == TRUE)
			{
				tree1.cur = link;
			}
			else
			{
				tree1.cur = temp;
			}
		}
	}
	return tree1;
}

// Changes the current pointer to the parent of the current.
tree parent(tree tree1)
{	
	tree1.cur = tree1.cur->par;
	return tree1;
}

// Prints the parse tree in outputparser.txt.
void printParseTree(FILE * fp)
{
	printTree(tr,fp);	
}

// Recursive function called through printParseTree for printing all nodes.
void printTree(tree tree1,FILE *fp)
{	
	Link temp = (Link)malloc(sizeof(Node));
	Link temp2 = (Link)malloc(sizeof(Node));
	temp = tree1.cur;
	if(tree1.cur->flag == TRUE)
	{	
		fprintf(fp,"%s -- ( ",tree1.cur->tokenstr);
		if(tree1.cur->child != NULL)
		{
			tree1.cur = tree1.cur->child;
			fprintf(fp,"%s ",tree1.cur->tokenstr);
			while(tree1.cur->next != NULL)
			{
				fprintf(fp," %s ",tree1.cur->next->tokenstr);
				tree1.cur = tree1.cur->next;
			}
		}
		fprintf(fp,")\n");
		tree1.cur = temp;
		temp2 = tree1.cur;
		if(tree1.cur->child != NULL)
		{
			tree1.cur = temp->child;
			printTree(tree1,fp);
		}
		tree1.cur = temp2;
		if(tree1.cur->next != NULL)
		{	
			tree1.cur = temp->next;
			printTree(tree1,fp);
		}
		tree1.cur = temp2;
	}
	else
	{	
		if(tree1.cur->next != NULL)
		{	
			tree1.cur=temp->next;
			printTree(tree1,fp);
		}
		tree1.cur = temp;
	}
}

