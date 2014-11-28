#include"stage4.h"
#include<string.h>
int main(int argc,char** argv)
{
	tree tr;
	astLink a;
	FILE *fp1,*fp2,*fp3;
	initializeScanner(argv[1]);
	fp1=fopen("1.txt","w");
	printTokenStream(fp1);
	reInitialize();
	createTree();
	initialiseSymTable();
	tr = Parse();
	fp2 = fopen("2.txt","w");
	printSymTab("3.txt");
	printParseTree(fp2);
	fp3 = fopen("4.txt","w");
	fclose(fp2);
	a=buildAst(tr.cur);
        printAstTree(fp3);
	opentargetfile(a);
	//buildIR(a);
	fcloseall();
	return 0;
}

