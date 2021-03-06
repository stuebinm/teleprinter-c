#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "charv.h"
#include "document.h"
#include "parsing.h"

char* otag_method (struct document* doc, struct macro* macro, struct charv** argv);
char* ctag_method (struct document* doc, struct macro* macro, struct charv** argv);

char* file_method (struct document* doc, struct macro* macro, struct charv** argv);

char* newcommand_method (struct document* doc, struct macro* macro, struct charv** argv);
char* custom_method (struct document* doc, struct macro* macro, struct charv** argv);

char* include_doc_method (struct document* doc, struct macro* macro, struct charv** argv);

char* begin_env_method (struct document* doc, struct macro* macro, struct charv** argv);
char* end_env_method (struct document* doc, struct macro* macro, struct charv** argv);

char* message_method (struct document* doc, struct macro* macro, struct charv** argv);
char* error_method (struct document* doc, struct macro* macro, struct charv** argv);

char* ifthenelse_method (struct document* doc, struct macro* macro, struct charv** argv);

#endif
