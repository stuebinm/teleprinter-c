#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>



void parse_main_loop (struct document* doc) {
	int column_count = 0;
	while ((layer_fetchc (doc->base)) != EOF) {
		if (skip_whitespace (doc)) {
			if (column_count > 100) {
				doc->printc ('\n');
				column_count = 0;
			} else doc->printc (' ');
		}
		switch (doc->c) {
			case '{':
				printf ("[word: %s]", parse_statement (doc)->array);
				break;
			case '\\':
				parse_command (doc);
				break;
			default:
				doc->printc ((char) doc->c);
				break;
		}
		column_count++;
	}
	
}

struct charv* parse_statement (struct document* doc) {
	struct charv* ret = new_charv (10);
	while ( (layer_fetchc (doc->base))) {
		if (doc->c == '{') {
			parse_statement (doc);
			continue;
		}
		else if (doc->c == '}') break;
		else if (doc->c == EOF) parse_error ();
		charv_append (ret, doc->c);
	}
	charv_finalize (ret);
	return ret;
}

void skip_comment (struct document* doc) {
	while ( (layer_fetchc (doc->base)) ) {
		if (doc->c == '\n') return;
		if (doc->c == EOF) parse_error ();
	}
}

int skip_whitespace (struct document* doc) {
	int s = 0;
	do {
	switch (doc->c) {
		case '%':
			skip_comment (doc);
			s = 1;
			continue;
		case ' ':
		case '\n':
		case '\t':
			s = 1;
			continue;
		default:
			return s;
	}
	} while ( (layer_fetchc (doc->base)));
}

void parse_command (struct document* doc) {
	struct command* command;
	int argc = 0;
	struct charv* name = new_charv(10);
	struct charv* arg0 = 0;
	while ( (layer_fetchc (doc->base)) ) {
		switch (doc->c) {
			case '{':
				arg0 = parse_statement (doc);
				//argc++;
			case ' ':
			case '\t':
			case '\n':
				//command = get_command ();
				charv_finalize (name);
				printf ("[command: %s]", name->array);
				goto endwhile;
			default:
				charv_append (name, (char) doc->c);
		}
	}
	endwhile:
	if (arg0 != 0) {
		printf ("[arg0: %s]", arg0->array);
		charv_free (arg0);
	}
	charv_free (name);
}

char** find_arguments (struct document* doc, int argc) {
	parse_error ();
}

void parse_error () {
	printf ("error while parsing …\n");
	exit (1);
}
