#ifndef PARSING_H
#define PARSING_H

#include "document.h"
#include "charv.h"

void parse_main_loop (struct document* doc);

struct charv* parse_statement (struct document* doc);

void skip_comment (struct document* doc);

int skip_whitespace (struct document* doc);

void parse_command (struct document* doc);

char** find_arguments (struct document* doc, int argc);

void parse_error ();

#endif
