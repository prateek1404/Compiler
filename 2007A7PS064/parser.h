// A simplified parser for the given grammar. It takes token stream as input and constructs and prints a parse tree.The tree can be visualized as a parent
// node containing the pointer to the head of the linked list containing the children nodes of that parent node.
#include"scanner.h"

struct node{					// represents a node in the parse tree.
	char *tokenstr;				// stores the token type.
	char *value;				// stores the value of the identifier or the number itself.
	boolean flag;				// determines whether the node stores a terminal or a nonterminal.
	struct node *par;			// stores a pointer to the parent node.
	struct node *child;			// stores the pointer to the child node.
	struct node *next;			// stores the pointer to the next node in the linked list of children.
	struct node *left;
	int lineno;
};

typedef struct node Node;
typedef Node *Link;

struct tree{					// stores the tree structure.
	Link root;				// stores the pointer to the root node.
	Link cur;				// stores the pointer to the present/current node.
};
typedef struct tree tree;

extern tree Parse();				// constructs a leftmost derivation of the given character stream and forms a parse tree.
extern void createTree();			// initializes the root node and makes the current node point to the root node.
extern void printParseTree(FILE *);		// prints out the parse tree in a file.
void error();							// Function to print error.
void etrans();							// Function to insert a node when an e-transition occurs.
void eat(tokenType t);						// Function to take a terminal(token) and advance.
void advance();							// Function to advance when a terminal(token) has been eaten up.
void createTree();
tree parent(tree);
tree insertNode(tree,char *,boolean,int);
char * getTokenName(tokenType);
void printTree(tree,FILE * fp);

// Nonterminal Function Declarations.
void Programs();
void Functions();
void MainFun();
void OtherFuns();
void FunBody();
void Funct();
void ResultType();
void Parameters();
void Parameter();
void RemainingParameters();
void Declarations();
void OtherStatements();
void Declaration();
void FunDeclTypes();
void FunDeclTypesRest();
void Statement();
void AssignmentStmt();
void ConditionalStmt();
void RepetitiveStmt();
void ReturnStmt();
void IO_Stmt();
void FunctionCallStmt();
void Expression();
void Term();
void EPrime();
void Factor();
void TPrime();
void FunctionCall();
void ActualParams();
void ActualParamsRest();
void BoolExp();
void BoolOps();
void ReturnVal();
void APrime();
