#define MAXPAR 10
#define MAXST 1000

typedef struct
{
	int r;
        int loc;
        int flag;//0->it exists in the table
}
hashstruct;

typedef enum
{
        TNONE = 0,TINT = 1
}
type;

typedef enum
{
        STV=0,STF=1,STU=2
}
stype;


typedef struct
{
        type t;
        int lineno;
//        char * value;
        int width;
}
symtabvar;


typedef struct
{
        type ret;
        int numarg;
        type * par;
	int dec;
}
symtabfun;

typedef union
{
        symtabvar * stv;
        symtabfun *stf;
}
symtabent;

typedef struct
{
        symtabent e;
        stype s;
	char * name;
	char * encfun;
}
symtab;

typedef symtab ST[MAXST];
                                     
