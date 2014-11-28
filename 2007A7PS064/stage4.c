#include "stage4.h"

FILE *fp;

//tempvar temp[100];
int count=0;
int flag1=0;
int counter=0;
int printCount=0;
void buildIR(astLink a)
{


         astLink k=a->child;

	if(a->nodetype==aIntVar)
	{
		fprintf(fp,"%s dw ?\n",a->child->value);

	}
	if(a->nodetype==aOUT_Stmt)
	{
		if(flag1==0)
		{
			fprintf(fp,".CODE\n.STARTUP\n");
			flag1=1;

		}

	printCount++;
	printf("printing %s\n",a->child->value);
	fprintf(fp,"MOV BX,%s\n",a->child->value);
	fprintf(fp,"MOV CX,BX\n");
	fprintf(fp,"CMP CX,0\n");
	fprintf(fp,"JGE tempi%d\n",printCount);
	fprintf(fp,"MOV AH,2\nINT 21h\nNEG BX\n");
	fprintf(fp,"tempi%d:\n",printCount);
	fprintf(fp,"call printid\n\n");

	}
	if(a->nodetype==aIN_Stmt)
	{
		if(flag1==0)
		{
			fprintf(fp,".CODE\n");
			fprintf(fp,".STARTUP\n");

		}

		fprintf(fp,"call getid\n");
		fprintf(fp,"MOV %s,BX\n\n",a->child->value);
	}

	if(a->nodetype==aRepetitiveStmt)
	{
		if(flag1==0)
		{
			fprintf(fp,".CODE\n");
			fprintf(fp,".STARTUP");
			flag1=1;

		}
	    int replab;
	    int another;
	    replab=counter;

            fprintf(fp,"LABEL%d:\n",counter);
            counter++;

            if(a->child->nodetype==aEqualityExp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"CMP BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JNZ LABEL%d \n",counter);
	      another=counter;
              counter++;

            }
             if(a->child->nodetype==aNotEqualityExp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"CMP BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JZ LABEL%d \n",counter);
	      another=counter;
              counter++;

            }
            if(a->child->nodetype==aLT_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JGE LABEL%d \n",counter);
	      another=counter;
              counter++;

            }
            if(a->child->nodetype==aLE_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JG LABEL%d \n",counter);
	      another=counter;
              counter++;

            }
            if(a->child->nodetype==aGT_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JLE LABEL%d \n",counter);
	      another=counter;
              counter++;

            }
            if(a->child->nodetype==aGE_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JL LABEL%d \n",counter);
	      another=counter;
              counter++;

            }

            buildIR(a->child->next);
            fprintf(fp,"JMP LABEL%d \n",replab);
            fprintf(fp,"LABEL%d:\n",another);



    }




	if(a->nodetype==aConditionalStmt)
        {

		int temp;
		if(flag1==0)
		{
			fprintf(fp,".CODE \n");
			fprintf(fp,".STARTUP\n");
			flag1=1;

		}
                a->child->quad.dest=""; // temporary boolean variaBLe
                // still to be done
                a->child->quad.op="";
                a->child->quad.operand1="";
	//	fprintf(fp,"bz ");
            char *buffer;
            if(a->child->nodetype==aEqualityExp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"CMP BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JNZ LABEL%d \n",counter);
	      temp=counter;
              counter++;

            }
             if(a->child->nodetype==aNotEqualityExp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"CMP BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JZ LABEL%d \n",counter);
		temp=counter;
              counter++;

            }
            if(a->child->nodetype==aLT_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JGE LABEL%d \n",counter);
	      temp=counter;
              counter++;

            }
            if(a->child->nodetype==aLE_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JG LABEL%d \n",counter);
	      temp=counter;
              counter++;

            }
            if(a->child->nodetype==aGT_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB BX,CX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JLE LABEL%d \n",counter);
	      temp=counter;
              counter++;

            }
            if(a->child->nodetype==aGE_Exp)
            {
              fprintf(fp,"MOV BX,%s \n",a->child->child->value);
              fprintf(fp,"MOV CX,%s \n",a->child->child->next->value);
              fprintf(fp,"SUB CX,BX \n");

             // itoa(counter,buffer,10);
              fprintf(fp,"JL LABEL%d \n",counter);
	      temp=counter;
              counter++;

            }

            buildIR(a->child->next);
            fprintf(fp,"LABEL%d:\n",temp);

        }




         while(k!=NULL && a->nodetype!=aConditionalStmt && a->nodetype!=aRepetitiveStmt)
         {
                buildIR(k);
                k=k->next;
         }

         // now for the current



        if(a->nodetype==aAssignmentStmt)
        {
		if(flag1==0)
		{
			fprintf(fp,".CODE \n");
			fprintf(fp,".STARTUP\n");
			flag1=1;
		}
                a->quad.dest=a->child->value;
                a->quad.op="MOV";
                a->quad.operand1=a->child->next->quad.dest;
                a->quad.operand2=NULL;
		fprintf(fp,"MOV AX,%s \n",a->quad.operand1);
		fprintf(fp,"MOV %s,AX \n\n",a->quad.dest);
        }
	if(a->nodetype==aTK_NUM)
	{
		a->quad.dest=a->value;

	}
	if(a->nodetype==aTK_ID)
	{
		a->quad.dest=a->value;

	}

        if(a->nodetype==aPLUS_Expr)
        {
                //  temp[count].sno=count;
                  //temp[count].type="+";
		  //
		  a->quad.dest="DX";


		//  else
		  {
		  //	a->quad.dest="EX";
		  }
                  a->quad.operand1=a->child->quad.dest;
                  a->quad.operand2=a->child->next->quad.dest;
                  a->quad.op="+";
		  fprintf(fp,"MOV BX,%s \n",a->quad.operand1);
		  if(!strcmp(a->quad.operand2,a->quad.operand1))
		  {
		  	fprintf(fp,"POP DX\n");
		  }
		  fprintf(fp,"MOV CX,%s \n",a->quad.operand2);
		  fprintf(fp,"ADD BX,CX \n" );
		  fprintf(fp,"MOV DX,BX \n\n",a->quad.dest);
		  if(a->next!=NULL)
		  {
		  fprintf(fp,"PUSH DX\n\n");
			}

        }

        if(a->nodetype==aMINUS_Expr)
        {
                 // temp[count].sno=count;
                  //temp[count].type="+";

                  a->quad.dest="DX";// temporary variable;
                  a->quad.operand1=a->child->quad.dest;
                  a->quad.operand2=a->child->next->quad.dest;
                  a->quad.op="-";

		  fprintf(fp,"MOV BX,%s \n",a->quad.operand1);
		  {
		  //fprintf(fp,"");
		  }
		   if(!strcmp(a->quad.operand2,a->quad.operand1))
		  {
		  	fprintf(fp,"POP DX\n");
		  }

		  fprintf(fp,"MOV CX,%s \n",a->quad.operand2);
		  fprintf(fp,"SUB BX,CX \n" );
		  fprintf(fp,"MOV DX,BX \n\n");

        }

        if(a->nodetype==aTIMES_Expr)
        {
                //  temp[count].sno=count;
                  //temp[count].type="+";

                  a->quad.dest="DX";// temporary variable;
                  a->quad.operand1=a->child->quad.dest;
                  a->quad.operand2=a->child->next->quad.dest;
                  a->quad.op="*";
		  fprintf(fp,"MOV AX,%s \n",a->quad.operand1);
		   if(!strcmp(a->quad.operand2,a->quad.operand1))
		  {
		  	fprintf(fp,"POP DX\n");
		  }
		  fprintf(fp,"MOV BX,%s \n",a->quad.operand2);
		  fprintf(fp,"IMUL BX \n");
		  fprintf(fp,"MOV DX,AX \n\n");


        }
        if(a->nodetype==aDIV_Expr)
        {
                  //temp[count].sno=count;
                  //temp[count].type="+";

                  a->quad.dest="DX";// temporary variable;
                  a->quad.operand1=a->child->quad.dest;
                  a->quad.operand2=a->child->next->quad.dest;
                  a->quad.op="/";
		  fprintf(fp,"MOV AX,%s \n",a->quad.operand1);
		   if(!strcmp(a->quad.operand2,a->quad.operand1))
		  {
		  	fprintf(fp,"POP DX\n");
		  }
		  fprintf(fp,"MOV BX,%s \n",a->quad.operand2);
		  fprintf(fp,"IDIV BX \n");
		  fprintf(fp,"MOV DX,AX\n\n");
		//  fprintf(fp,"MOV DX,BX");


        }
        // conditional statements


}

void opentargetfile(astLink a)
{
        fp=fopen("x86code.asm","w");
	fprintf(fp,".MODEL TINY\n");
        fprintf(fp," .DATA \n");
        fprintf(fp,"temp dw ?\n");
	buildIR(a);
	fprintf(fp,".EXIT \nEND\n");
	fprintf(fp,"\ngetid proc near\n");
fprintf(fp,"MOV BX,0\nMOV temp,BX\n");
fprintf(fp,"MOV cx,0\nMOV ah,01h\nINT 21h\n");
fprintf(fp,"MOV BL,al\ncmp BL,'-'\njne lab1\nMOV temp,1\n");
fprintf(fp,"lab2:\nMOV ah,01h\nint 21h\nlab1:\nMOV BL,al\ncmp BL,13\n");
fprintf(fp,"je lab3\nMOV ah,0\npush ax\nadd cx,1\njmp lab2\n");
fprintf(fp,"lab3:\nMOV BX,0\nMOV bp,1\nlab4:\npop si\nsub si,'0'\n");
fprintf(fp,"MOV di,si\nMOV ax,bp\nimul di\nadd BX,ax\nMOV ax,bp\nMOV di,10\nimul di\nMOV bp,ax\nsub cx,1\njnz lab4\n");
fprintf(fp,"MOV ax,temp\nsub ax,0\nje lab5\nneg BX\nlab5:\nMOV dl,10\nMOV ah,2\nint 21h\n");
fprintf(fp,"MOV dl,13\nMOV ah,2\nint 21h\n");
fprintf(fp,"ret\ngetid endp\n");

fprintf(fp,"\nprintid proc near\n");
fprintf(fp,"MOV si,0\nMOV cx,10\n");
fprintf(fp,"lBL1:\nMOV dx,0\nMOV ax,BX\nidiv cx\nadd dx,'0'\npush dx\nadd si,1\nMOV BX,ax\nsub BX,0\njnz lBL1\n");
fprintf(fp,"lBL2:\npop dx\nMOV ah,2\nint 21h\nsub si,1\njnz lBL2\n");
fprintf(fp,"MOV dl,10\nMOV ah,2\nint 21h\n");
fprintf(fp,"MOV dl,13\nMOV ah,2\nint 21h\n");
fprintf(fp,"ret\nprintid endp\n");
fprintf(fp, "\nend\n");


}
