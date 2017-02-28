#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "charv.h"
#include "macrostack.h"

char* enquote_method (struct macro* macro, struct charv** argv);
char* chapter_method (struct macro* macro, struct charv** argv);
char* chbegin_method (struct macro* macro, struct charv** argv);
char* part_method (struct macro* macro, struct charv** argv);

char* newcommand_method (struct macro* macro, struct charv** argv);
char* custom_method (struct macro* macro, struct charv** argv);


#endif
