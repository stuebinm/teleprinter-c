#ifndef TELEPRINTER_H
#define TELEPRINTER_H

#include "charv.h"

void print_document (struct charv* prefix, struct charv* workingdir, FILE* log, FILE* err);

//void print_document (FILE* input, FILE* output, FILE* log, FILE* err);


#endif
