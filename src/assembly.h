#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "charv.h"
#include "macrostack.h"

char* newcommand_method (struct macro* macro, struct charv** argv);
char* custom_method (struct macro* macro, struct charv** argv);


#endif
