#include"ast.h"
#include<string.h>

char astname[][25]={"Program","MainFun","Funct","Declaration", "IntVar", "FunctType", "ResultType", "Parameters", "FormalParamList", 
			"Statement", "AssignmentStmt","Expr","PLUS_Expr", "MINUS_Expr", "TIMES_Expr", "DIV_Expr", "FunctionCall", "ActualParams",
			 "ParamsList", "ConditionalStmt", "BoolExp","EqualityExp",  "NotEqualityExp", "LT_Exp", "GT_Exp", "LE_Exp", "GE_Exp",
			 "RepetitiveStmt", "ReturnStmt", "FunctionCallStmt", "IO_Stmt","IN_Stmt", "OUT_Stmt","TK_KEY_INT","TK_KEY_NONE",
			"TK_ID","TK_NUM"
			 };
Link link;
FILE* fp;
astLink astRoot;
extern void printAst(astLink tree1,FILE *fp);	
extern void printAstTree(FILE * fp);
extern astLink astProgram(Link ln);
extern astLink astMainFun(Link ln);
extern astLink astFunct(Link ln);
extern astLink astDeclaration(Link ln);
extern astLink astIntVar(Link ln);
extern astLink astFunctType(Link ln);
extern astLink astResultType(Link ln);
extern astLink astParameters(Link ln);
extern astLink astFormalParamList(Link ln);
extern astLink astStatement(Link ln);
extern astLink astAssignmentStmt(Link ln);
extern astLink astExpr(Link ln);
extern astLink astPLUS_Expr(Link ln);
extern astLink astMINUS_Expr(Link ln);
extern astLink astTIMES_Expr(Link ln);
extern astLink astDIV_Expr(Link ln);
extern astLink astFunctionCall(Link ln);
extern astLink astActualParams(Link ln);
extern astLink astParamsList(Link ln);
extern astLink astConditionalStmt(Link ln);
extern astLink astBoolExp(Link ln);
extern astLink astEqualityExp(Link ln);
extern astLink astNotEqualityExp(Link ln);
extern astLink astLT_Exp(Link ln);
extern astLink astGT_Exp(Link ln);
extern astLink astLE_Exp(Link ln);
extern astLink astGE_Exp(Link ln);
extern astLink astRepetitiveStmt(Link ln);
extern astLink astReturnStmt(Link ln);
extern astLink astFunctionCallStmt(Link ln);
extern astLink astIO_Stmt(Link ln);
extern astLink astIN_Stmt(Link ln);
extern astLink astOUT_Stmt(Link ln);
extern astLink astID(Link ln,char * a);
extern astLink astNUM(Link ln);
extern astLink aFactor(Link l);
extern astLink aTPrime(Link l);
extern astLink aTerm(Link l);
extern astLink aEPrime(Link l);

astLink createAstNode()
{
	astLink link;
	link = (astLink)malloc(sizeof(astNode));
	link->nodetype = -1;
	link->value = NULL;
	link->value =(char *)malloc(sizeof(char)*15);
	link->flag = FALSE;
	link->par = NULL;
	link->child = NULL;
	link->next = NULL;
	return link;
}

astLink buildAst(Link ln)
{
	astRoot=astProgram(ln);
	return astRoot;
}

astLink astProgram(Link ln)
{
	astLink temp,temp1;
	astRoot = createAstNode();
	astRoot->nodetype = aProgram;
	ln = ln->child->child;											// To Functions
	while(ln->child!=NULL && (strcmp(ln->child->tokenstr,"e") != 0))
	{
		if(astRoot->child == NULL)
		{
			temp = astMainFun(ln->child);							// pass MainFun
			temp->par = astRoot;
			astRoot->child = temp;
		}
		else
		{
			temp = astFunct(ln->child);
			temp->par = astRoot;
			temp1 = astRoot->child;
			while(temp1->next != NULL)
			{
				temp1 = temp1->next;
			}
			temp1->next = temp;
		}
		ln = ln->child->next;
	}
	return astRoot;
}

astLink astMainFun(Link ln)
{
	boolean flag1=FALSE;
	astLink temp,temp1,temp2;
	Link lntemp;
	temp = createAstNode();
	temp->nodetype = aMainFun;
	ln = ln->child->next->next;														// at FunBody
	lntemp = ln;
	ln=ln->child;
	while((ln != NULL) && (strcmp(ln->child->tokenstr,"e")!=0))
	{
		if((strcmp(ln->child->child->tokenstr,"TK_KEY_INT") == 0) || (strcmp(ln->child->child->tokenstr,"TK_KEY_FUN") == 0))
		{
			temp1 = astDeclaration(ln->child);								// pass Declaration
			if(temp->child == NULL)
			{
				temp->child = temp1;
			}
			temp1->par = temp;
			temp2 = temp->child;
			if(flag1)
			{
				while(temp2->next != NULL)
				{
					temp2 = temp2->next;
				}
				temp2->next = temp1;
			}
			flag1=TRUE;
		}
		ln = ln->child->next;
	}
	ln = lntemp;														// at FunBody
	ln = ln->child->next;
	while((ln != NULL) && (strcmp(ln->child->tokenstr,"e")!=0))
	{
		temp1 = astStatement(ln->child);										// pass Statement
		if(temp->child == NULL)
		{
			temp->child = temp1;
		}
		temp1->par = temp;
		temp2 = temp->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp1;
		ln = ln->child->next;
	}
	temp2=temp->child;
	while(temp2->next != NULL)
		{	
			temp2 = temp2->next;
		}
	return temp;
}

astLink astFunct(Link ln)
{
	astLink temp,temp1,temp2,temp3;
	Link lntemp;
	temp = createAstNode();
	temp->nodetype = aFunct;
	ln = ln->child->next;
	temp3 = astID(ln,"FunctName");												// pass TK_ID
	temp3->par = temp;
	temp->child = temp3;
	
	ln = ln->next->next;													// at ResultType
	temp1 = astResultType(ln);												// pass ResultType
	temp1->par = temp;
	temp2 = temp->child;
	while(temp2->next != NULL)
	{
		temp2 = temp2->next;
	}
	temp2->next = temp1;
	
	ln = ln->next->next;													// at Parameters
	temp1 = astParameters(ln);												// pass Parameters
	temp1->par = temp;
	temp2 = temp->child;
	while(temp2->next != NULL)
	{
		temp2 = temp2->next;
	}
	temp2->next = temp1;
	
	ln = ln->next->next->next;												// at FunBody
	lntemp = ln;
	ln=ln->child;
	while(ln != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		if((strcmp(ln->child->child->tokenstr,"TK_KEY_INT") == 0) || (strcmp(ln->child->child->tokenstr,"TK_KEY_FUN") == 0))
		{
			temp1 = astDeclaration(ln->child);								// pass Declaration
			temp1->par = temp;
			temp2 = temp->child;
			while(temp2->next != NULL)
			{
				temp2 = temp2->next;
			}
			temp2->next = temp1;
		}
		ln = ln->child->next;
	}
	ln = lntemp;														// at FunBody
	
	ln = ln->child->next;
	while(ln != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		temp1 = astStatement(ln->child);										// pass Statement
		temp1->par = temp;
		temp2 = temp->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp1;
		ln = ln->child->next;
	}	
	return temp;
}

astLink astDeclaration(Link ln)
{
	astLink temp,temp1;
	if(strcmp(ln->child->tokenstr,"TK_KEY_INT") == 0)
	{
		temp1 = astIntVar(ln->child->next);										// pass TK_ID
	}
	else if(strcmp(ln->child->tokenstr,"TK_KEY_FUN") == 0)
	{
		ln = ln->child->next->next->next;										// at FunDeclTypes
		temp1 = astFunctType(ln);											// pass FunDeclTypes
	}
	return temp1;
}
		
astLink astIntVar(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aIntVar;
	temp1=astID(ln,"var");
	temp1->par=temp;
	temp->child=temp1;
	temp->flag=0;
	strcpy(temp->value,ln->tokenstr);
	return temp;
}
		
astLink astFunctType(Link ln)
{
	astLink temp,temp1,temp2;
	temp = createAstNode();
	temp->nodetype = aFunctType;
	temp1 = astID(ln->par->child->next,"FunctName");									// pass TK_ID
	temp1->par = temp;
	temp->child = temp1;
	temp1 = astResultType(ln->next->next->next);										// pass ResultType
	temp1->par = temp;
	temp->child->next = temp1;
	if(strcmp(ln->child->tokenstr,"TK_KEY_NONE") == 0)
	{
		temp1 = (astLink)malloc(sizeof(astNode));
		temp1->nodetype = aTK_KEY_NONE;
		temp1->par = temp;
		temp1->flag = 1;
		temp->child->next->next = temp1;
		return temp;
	}
	else
	{
		temp1 = (astLink)malloc(sizeof(astNode));
		temp1->nodetype = aTK_KEY_INT;
		temp1->flag=1;
		temp1->par = temp;
		temp2 = temp->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp1;
		ln = ln->child->next;
	}
	while(ln != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		temp1 = (astLink)malloc(sizeof(astNode));
		temp1->nodetype = aTK_KEY_INT;
		temp1->flag=1;
		temp1->par = temp;
		temp2 = temp->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp1;
		ln = ln->child->next->next;
	}
	return temp;
}

astLink astResultType(Link ln)
{
	astLink temp,temp1;
	temp1 = (astLink)malloc(sizeof(astNode));
	if(strcmp(ln->child->tokenstr,"TK_KEY_NONE") == 0)
	{
		temp1->nodetype = aTK_KEY_NONE;
	}
	else
	{
		temp1->nodetype = aTK_KEY_INT;
	}
	temp1->flag = 1;
	return temp1;
}

astLink astParameters(Link ln)
{
	astLink temp,temp1,temp3;
	temp1 = createAstNode();
	if(strcmp(ln->child->tokenstr,"TK_KEY_NONE") == 0)
	{
		temp1->nodetype = aTK_KEY_NONE;
		temp1->flag=1;
		return temp1;
	}
	temp1 = astID(ln->child->child->next,"FormalParams");									// pass TK_ID
	ln = ln->child->next;													// at RemainingParameters
	temp3 = temp1;
	temp = temp3;
	while(ln != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		temp1 = astFormalParamList(ln);											// pass ReamainingParameters
		temp3->next = temp1;
		temp3 = temp3->next;
		ln = ln->child->next->next;
	}
	return temp;
}

astLink astFormalParamList(Link ln)
{
	astLink temp,temp1;
	ln = ln->child->next->child->next;											// at TK_ID
	temp1 = astID(ln,"FormalParams");											// pass TK_ID
	return temp1;
}

astLink astStatement(Link ln)
{
	astLink temp,temp1;
	if(strcmp(ln->child->tokenstr,"AssignmentStmt") == 0)
	{
		temp1 = astAssignmentStmt(ln->child);										// pass AssignmentStmt
	}
	else if(strcmp(ln->child->tokenstr,"ConditionalStmt") == 0)
	{
		temp1 = astConditionalStmt(ln->child);										// pass ConditionalStmt
	}	
	else if(strcmp(ln->child->tokenstr,"RepetitiveStmt") == 0)
	{
		temp1 = astRepetitiveStmt(ln->child);										// pass RepetitiveStmt
	}
	else if(strcmp(ln->child->tokenstr,"ReturnStmt") == 0)
	{
		temp1 = astReturnStmt(ln->child);										// pass ReturnStmt
	}
	else if(strcmp(ln->child->tokenstr,"IO_Stmt") == 0)
	{
		temp1 = astIO_Stmt(ln->child);											// pass IO_Stmt
	}
	else
	{
		temp1 = astFunctionCallStmt(ln->child);										// pass FunctionCallStmt
	}
	return temp1;
}
	
astLink astAssignmentStmt(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aAssignmentStmt;
	ln = ln->child;														// at TK_ID
	temp1 = astID(ln,"left");												// pass TK_ID
	temp1->par = temp;
	temp->child = temp1;
	ln = ln->next->next;													// at Expression
	temp1 = astExpr(ln);													// pass Expression
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}
	
astLink astFunctionCall(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aFunctionCall;
	ln = ln->child->next;													// at TK_ID
	temp1 = astID(ln,"FunctName");												// pass TK_ID
	temp1->par = temp;
	temp->child = temp1;
	ln = ln->next->next;													// at Expression
	temp1 = astActualParams(ln);												// pass Expression
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}

astLink astActualParams(Link ln)
{
	astLink temp,temp1;
	temp1 = createAstNode();
	if(strcmp(ln->child->tokenstr,"TK_KEY_NONE") == 0)
	{
		temp1->nodetype = aTK_KEY_NONE;
		temp1->flag=1;
		return temp1;
	}
	ln = ln->child;														// at TK_ID or TK_NUM
	if(strcmp(ln->tokenstr,"TK_ID") == 0)
	{
		temp1 = astID(ln,"ParamName");											// pass TK_ID
	}
	else
	{
		temp1 = astNUM(ln);												// pass TK_NUM
	}
	temp1->next= astParamsList(ln->next);											// pass ActualParamsRest
	return temp1;
}

astLink astParamsList(Link ln)
{
	astLink temp,temp1,temp2;
	boolean i=FALSE;
	while(ln != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		ln = ln->child->next->child;											// at TK_ID or TK_NUM
		if(strcmp(ln->tokenstr,"TK_ID") == 0)
		{
			temp1 = astID(ln,"ParamName");										// pass TK_ID
		}
		else
		{
			temp1 = astNUM(ln);											// pass TK_NUM
		}
		temp2 = temp;
		if(i!=FALSE)
		{
			while(temp2->next != NULL)
			{
				temp2 = temp2->next;
			}
			temp2->next = temp1;
		}
		else
		{
			temp=temp1;
		}
		i=TRUE;
		ln = ln->next;													// at ActualParamsRest
	}
	return temp;
}

astLink astConditionalStmt(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aConditionalStmt;
	ln=ln->child->next->next;
	temp = astBoolExp(ln);
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next;
	while(ln != NULL && ln->child != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		temp = astStatement(ln->child);
		temp->par = temp1;
		temp2 = temp1->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp;
		temp->par = temp1;
		ln = ln->child->next;
	}
	return temp1;
}
		
astLink astRepetitiveStmt(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aRepetitiveStmt;
	ln=ln->child->next->next;
	temp = astBoolExp(ln);
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next;
	while(ln != NULL && ln->child != NULL && strcmp(ln->child->tokenstr,"e")!=0)
	{
		temp = astStatement(ln->child);
		temp->par = temp1;
		temp2 = temp1->child;
		while(temp2->next != NULL)
		{
			temp2 = temp2->next;
		}
		temp2->next = temp;
		temp->par = temp1;
		ln = ln->child->next;
	}
	return temp1;
}	

astLink astOUT_Stmt(Link ln)
{
	astLink temp,temp1;
	temp1 = createAstNode();
	temp1->nodetype = aOUT_Stmt;
	temp = astID(ln,"output");
	temp->par = temp1;
	temp1->child = temp;
	return temp1;
}
astLink astIN_Stmt(Link ln)
{
	astLink temp,temp1;
	temp1 = createAstNode();
	temp1->nodetype = aIN_Stmt;
	temp = astID(ln,"input");
	temp->par = temp1;
	temp1->child = temp;
	return temp1;
}	

astLink astIO_Stmt(Link ln)
{
	astLink temp,temp1,temp2;
	ln=ln->child;
	if(strcmp(ln->tokenstr,"TK_KEY_PRINT")==0)
	{
		ln=ln->next;
		temp = astOUT_Stmt(ln);
	}
	else
	{
		ln=ln->next;
		temp = astIN_Stmt(ln);
	}
	return temp;
}

astLink astFunctionCallStmt(Link ln)
{
	astLink temp,temp1,temp2;
	ln=ln->child;
	temp = astFunctionCall(ln);
	return temp;
}	

astLink astReturnStmt(Link ln)
{
	astLink temp,temp1,temp2;
	ln=ln->child->next->child;
	if(strcmp(ln->tokenstr,"TK_NUM")==0)
	{
		temp = astNUM(ln);
	}
	else
	{
		temp = astID(ln,"return");
	}
	return temp;
}

astLink astGE_Exp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aGE_Exp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}

astLink astLE_Exp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aLE_Exp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}

astLink astGT_Exp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aGT_Exp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}

astLink astLT_Exp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aLT_Exp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}

astLink astEqualityExp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aEqualityExp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}	

astLink astNotEqualityExp(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aNotEqualityExp;
	ln=ln->par->par->child;
	temp = astID(ln,"left");
	temp->par = temp1;
	temp1->child = temp;
	ln=ln->next->next->next->next;
	temp2 = astID(ln,"right");
	temp2->par = temp1;
	temp->next = temp2;
	return temp1;
}

astLink astBoolExp(Link ln)
{
	astLink temp,temp1,temp2;
	ln=ln->child->next->next->child;
	if(strcmp(ln->tokenstr,"TK_EQUAL")==0)
	{
		temp=astEqualityExp(ln);
	}
	else if(strcmp(ln->tokenstr,"TK_NOTEQUAL")==0)
	{
		temp=astNotEqualityExp(ln);
	}
	else if(strcmp(ln->tokenstr,"TK_LT")==0)
	{
		temp=astLT_Exp(ln);
	}
	else if(strcmp(ln->tokenstr,"TK_GT")==0)
	{
		temp=astGT_Exp(ln);
	}
	else if(strcmp(ln->tokenstr,"TK_LEQ")==0)
	{
		temp=astLE_Exp(ln);
	}
	else if(strcmp(ln->tokenstr,"TK_GEQ")==0)
	{
		temp=astGE_Exp(ln);
	}
	return temp;

}

// Prints the ast tree in outputast.txt.
void printAstTree(FILE * fp)
{
	printAst(astRoot,fp);
		
}

// Recursive function called through printAstTree for printing all nodes.
void printAst(astLink tree1,FILE *fp)	
{	
	char c[30]="\0";
	astLink temp = (astLink)malloc(sizeof(astNode));
	astLink temp2 = (astLink)malloc(sizeof(astNode));
	temp = tree1;
	if(tree1->flag == FALSE)
	{	
//		if(tree1->nodetype == TK_ID)
//		{
//			strcpy(c,astname[tree1->nodetype]);
//			strcat(c,tree1->value);
//			fprintf(fp,"%s -- ( ",c);
//		}
//		else
//		{
			fprintf(fp,"%s -- ( ",astname[tree1->nodetype]);
//		}
		if(tree1->child != NULL)
		{
			tree1 = tree1->child;
			if(tree1->nodetype == aTK_ID)
			{
			strcpy(c,astname[tree1->nodetype]);
			strcat(c,"_");
			strcat(c,tree1->value);
				fprintf(fp,"%s ",c);
			}
			else
			{
				fprintf(fp,"%s ",astname[tree1->nodetype]);
			}
			while(tree1->next != NULL)
			{
				if(tree1->next->nodetype == aTK_ID)
				{
					strcpy(c,astname[tree1->next->nodetype]);
					strcat(c,"_");
					strcat(c,tree1->next->value);
					fprintf(fp," %s ",c);
				}
				else
				{
					fprintf(fp," %s ",astname[tree1->next->nodetype]);
				}
				tree1 = tree1->next;
			}
		}
		fprintf(fp,")\n");
		tree1 = temp;
		temp2 = tree1;
		if(tree1->child != NULL)
		{
			tree1 = temp->child;
			printAst(tree1,fp);
		}
		tree1 = temp2;
		if(tree1->next != NULL)
		{	
			tree1 = temp->next;
			printAst(tree1,fp);
		}
		tree1 = temp2;
	}
	else
	{	
		if(tree1->next != NULL)
		{	
			tree1=temp->next;
			printAst(tree1,fp);
		}
		tree1 = temp;
	}
}

astLink astID(Link ln,char * a)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aTK_ID;
	strcpy(temp1->value,ln->value);
//	strcat(temp1->value,"_");
//	strcat(temp1->value,a);
	temp1->flag=1;
	return temp1;	
}


astLink astNUM(Link ln)
{
	astLink temp,temp1,temp2;
	temp1 = createAstNode();
	temp1->nodetype = aTK_NUM;
	strcpy(temp1->value,ln->value);
	temp1->flag=1;
	return temp1;	
}


astLink astExpr(Link ln)
{
	astLink temp,temp1;
	if(strcmp(ln->child->next->child->tokenstr,"e") == 0)
	{
		ln = ln->child;													// at Term
		if(strcmp(ln->child->next->child->tokenstr,"e") == 0)
		{
			ln = ln->child;												// at Factor
			temp1 = aFactor(ln);
			return temp1;
		}
		else																			// TPrime is not NULL
		{
			ln = ln->child->next->child;										// at * or /
			if(strcmp(ln->tokenstr,"TK_TIMES") == 0)
			{
				temp1 = astTIMES_Expr(ln);									// pass *
				return temp1;
			}
			else
			{
				temp1 = astDIV_Expr(ln);									// pass /
				return temp1;
			}
		}
	}
	else															// EPrime is not NULL
	{
		ln = ln->child->next->child;											// at + or -
		if(strcmp(ln->tokenstr,"TK_PLUS") == 0)
		{
			temp1 = astPLUS_Expr(ln);										// pass +
			return temp1;
		}
		else
		{
			temp1 = astMINUS_Expr(ln);										// pass -
			return temp1;
		}
	}
	return temp1;
}
astLink astTIMES_Expr(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aTIMES_Expr;
	
	ln = ln->par->par;
	if(strcmp(ln->tokenstr,"TPrime")==0)
	{
		ln=ln->child->next;
	}			
	else
	{
		ln=ln->child;
	}															// at Facto
	temp1 = aFactor(ln);													// pass Factor
	temp1->par = temp;
	temp->child = temp1;	
	temp1 = aTPrime(ln->next);												// pass TPrime
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}

astLink astDIV_Expr(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aDIV_Expr;	
	ln = ln->par->par;
	if(strcmp(ln->tokenstr,"TPrime")==0)
	{
		ln=ln->child->next;
	}			
	else
	{
		ln=ln->child;
	}																// at Factor
	temp1 = aFactor(ln);														// pass Factor
	temp1->par = temp;
	temp->child = temp1;
	
	temp1 = aTPrime(ln->next);													// pass TPrime
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}

astLink astPLUS_Expr(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aPLUS_Expr;
	
	ln = ln->par->par;
	if(strcmp(ln->tokenstr,"EPrime")==0)
	{
		ln=ln->child->next;
	}			
	else
	{
		ln=ln->child;
	}																// at Term
	temp1 = aTerm(ln);														// pass Term
	temp1->par = temp;
	temp->child = temp1;
	
	temp1 = aEPrime(ln->next);
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}

astLink astMINUS_Expr(Link ln)
{
	astLink temp,temp1;
	temp = createAstNode();
	temp->nodetype = aMINUS_Expr;
	
	ln = ln->par->par;
	if(strcmp(ln->tokenstr,"EPrime")==0)
	{
		ln=ln->child->next;
	}			
	else
	{
		ln=ln->child;
	}																// at Term
	temp1 = aTerm(ln);															// pass Term
	temp1->par = temp;
	temp->child = temp1;
	temp1 = aEPrime(ln->next);
	temp1->par = temp;
	temp->child->next = temp1;
	return temp;
}

astLink aFactor(Link ln)
{
	astLink temp;
	
	if(strcmp(ln->child->tokenstr,"TK_OPEN") == 0)
	{
		temp = astExpr(ln->child->next);											// pass Expression
	}
	else if(strcmp(ln->child->tokenstr,"TK_NUM") == 0)
	{
		temp = astNUM(ln->child);											// pass TK_NUM
	}
	else if(strcmp(ln->child->tokenstr,"TK_ID") == 0)
	{
		temp = astID(ln->child,"Name");												// pass TK_ID
	}
	else
	{
		temp = astFunctionCall(ln->child);										// pass FunctionCall
	}
	return temp;
}

astLink aTPrime(Link ln)
{
	astLink temp;
	if(strcmp(ln->child->next->next->child->tokenstr,"e")==0)
	{
		temp = aFactor(ln->child->next);											// pass Factor
	}
	else
	{
		ln = ln->child->next->next;												// at TPrime
		if(strcmp(ln->child->tokenstr,"TK_TIMES") == 0)
		{
			temp = astTIMES_Expr(ln->child);									// pass *
		}
		else
		{
			temp = astDIV_Expr(ln->child);										// pass /
		}
	}
	return temp;
}

astLink aTerm(Link ln)
{
	astLink temp;
	if(ln->child==NULL)
	printf("null\n");
	if(strcmp(ln->child->next->child->tokenstr,"e") == 0)
	{
		temp = aFactor(ln->child);												// pass Factor
	}
	else
	{
		ln = ln->child->next;													// at TPrime
		if(strcmp(ln->child->tokenstr,"TK_TIMES") == 0)
		{
			temp = astTIMES_Expr(ln->child);									// pass *
		}
		else
		{
			temp = astDIV_Expr(ln->child);										// pass /
		}
	}
	return temp;
}

astLink aEPrime(Link ln)
{
	astLink temp;
	if(strcmp(ln->child->next->next->child->tokenstr,"e")==0)
	{
		temp = aTerm(ln->child->next);											// pass Term
	}
	else
	{
		ln = ln->child->next->next;												// at EPrime
		if(strcmp(ln->child->tokenstr,"TK_PLUS") == 0)
		{
			temp = astPLUS_Expr(ln->child);										// pass +
		}
		else
		{
			temp = astMINUS_Expr(ln->child);									// pass -
		}
	}
	return temp;
}

