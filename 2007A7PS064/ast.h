#include"parser.h"

typedef struct
{
    char *dest;
    char *operand1;
    char *operand2;
    char *op;
} QUADRUPLE;


typedef enum { aProgram=0, aMainFun, aFunct, aDeclaration, aIntVar, aFunctType, aResultType, aParameters, aFormalParamList, aStatement, aAssignmentStmt, aExpr,
			   aPLUS_Expr, aMINUS_Expr, aTIMES_Expr, aDIV_Expr, aFunctionCall, aActualParams, aParamsList, aConditionalStmt, aBoolExp,
 			aEqualityExp,  aNotEqualityExp, aLT_Exp, aGT_Exp, aLE_Exp, aGE_Exp, aRepetitiveStmt, aReturnStmt, aFunctionCallStmt, aIO_Stmt,
			 aIN_Stmt, aOUT_Stmt,aTK_KEY_INT,aTK_KEY_NONE,aTK_ID,aTK_NUM
			 }NodeType;
struct astnode					// represents a node in the AST tree.
{
	NodeType nodetype;			// stores the token type.
	char *value;				// stores the value of the identifier or the number itself.
	boolean flag;				// determines whether the node stores a terminal or a nonterminal.
	struct astnode *par;			// stores a pointer to the parent node.
	struct astnode *child;			// stores the pointer to the child node.
	struct astnode *next;
	QUADRUPLE quad;			// stores the pointer to the next node in the linked list of children.
};
typedef struct astnode astNode;
typedef astNode *astLink;

extern astLink buildAst(Link ln);
