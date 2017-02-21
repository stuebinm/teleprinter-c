#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "error.h"


void parse_main_loop (struct document* doc) {
	int column_count = 0;
	skip_whitespace (doc);
	while ((layer_fetchc (doc->base)) != EOF) {
		if (skip_whitespace (doc)) {
			if (column_count > 100) {
				doc->printc ('\n');
				column_count = 0;
			} else doc->printc (' ');
		}
		if (doc->c == EOF) break;
		switch (doc->c) {
			case '{': ;
				struct charv* word = parse_statement (doc);
				printf ("[word: %s]", word->array);
				charv_free (word);
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
	printf ("\n");
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
		char c = (char) doc->c;
		charv_append (ret, c);
		//printf ("%c\n", c);
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

struct charv** get_statements (struct document* doc, int number) {
	struct charv** ret = malloc (number * sizeof (struct charv));
	for (int i = 0; i<number; i++) {
	printf ("\nsearching for %d …\n", i);
		do {
			switch (doc->c) {
				case '{':
					ret[i] = parse_statement (doc);
					goto cont;
				default:
					break;
			}
		}
		while (layer_fetchc (doc->base));
	cont: ;
	}
	return ret;
}

void parse_command (struct document* doc) {
	struct charv* name = new_charv(10);
	struct command* com;
	while ( (layer_fetchc (doc->base)) ) {
		switch (doc->c) {
			case '{':
			case ' ':
			case '\t':
			case '\n':
				charv_finalize (name);
				com = get_command (name->array);
				if (com == 0) {
					unknown_command_exit (name->array);
				}
				if (com->argc != 0) {
					printf ("command: %s\n", name->array);
					struct charv** argv = get_statements (doc, com->argc);
					doc->printf (com->method (argv));
					charv_array_free (argv, com->argc);
				}
				charv_free (name);
				return;
			case EOF:
				parse_error ();
			default:
				charv_append (name, (char) doc->c);
				break;
		}
	}
}

char** find_arguments (struct document* doc, int argc) {
	parse_error ();
}

void parse_error () {
	printf ("error while parsing …\n");
	exit (1);
}
