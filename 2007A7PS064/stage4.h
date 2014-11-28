#include "ast.h"

typedef struct
{
        char *type;
        int sno;

} tempvar;


extern void buildIR(astLink a);
extern void opentargetfile(astLink a);
