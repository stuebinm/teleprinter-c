#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>

#include "macrostack.h"
#include "error.h"

int d;

void parse_main_loop (struct document* doc) {
	int column_count = 0;
	skip_whitespace (doc);
	d++;
	do{
	    main_loop_start:
		if (skip_whitespace (doc)) {
			if (column_count > 100) {
				doc_putc (doc, '\n');
				column_count = 0;
			} else doc_putc (doc, ' ');
		}
		if (doc->c == EOF) break;
		switch (doc->c) {
			case '{': ;
				struct charv* word = parse_statement (doc);
				msg_log ("floating word", word->array);
				doc_print (doc, word->array);
				charv_free (word);
				break;
			case '}':
			    if (d == 0) parse_error();
			    return;
			case '\\':
				parse_command (doc);
				//document_fetchc (doc);
				goto main_loop_start;
				break;
			case '\n':
			    document_fetchc (doc);
			    if (doc->c != '\n') goto main_loop_start;
			    doc_print (doc, "\n</p>\n<p align=\"justify\">\n");
			default:
				doc_putc (doc, (char) doc->c);
				break;
		}
		column_count++;
	} while (document_fetchc (doc) != EOF);
	d--;
}

struct charv* parse_statement (struct document* doc) {
	struct charv* ret = new_charv (10);
	document_fetchc (doc);
	document_push_layer_str (doc, ret);
	parse_main_loop (doc);
	document_pop_layer (doc);
	charv_finalize (ret);
	return ret;
}

void skip_comment (struct document* doc) {
	while (document_fetchc (doc)) {
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
		//case '\n':
		case '\t':
			s = 1;
			continue;
		default:
			return s;
	}
	} while (document_fetchc (doc));
}

struct charv** get_statements (struct document* doc, int number) {
	struct charv** ret = malloc (number * sizeof (struct charv));
	for (int i = 0; i<number; i++) {
		do {
			switch (doc->c) {
				case '{':
					/*ret[i] = parse_statement (doc);
					document_fetchc (doc); // fetch the next char; parse_statement leaves at '}'!*/
					ret[i] = new_charv (10);
					int d = 1;
					while (document_fetchc (doc)) {
					    switch (doc->c) {
					        case '{':
					            d += 1;
					            charv_append (ret[i], doc->c);
					            break;
				            case '}':
				                d -= 1;
				                if (d == 0) {
				                    charv_finalize (ret[i]);
				                    document_fetchc (doc);
				                    goto cont;
				                }
				                charv_append (ret[i], doc->c);
				                break;
			                default:
			                    charv_append (ret[i], doc->c);
			                    
					    }
					}
					goto cont;
				case ' ':
				case '\n':
				case '\t':
					break;
				default: ;
				    struct charv* arg = new_charv (1);
				    msg_logc ("single char argument", doc->c);
				    charv_append (arg, doc->c);
				    charv_finalize (arg);
				    ret[i] = arg;
				    goto cont;
			}
		}
		while (document_fetchc (doc));
	cont: ;
	}
	return ret;
}

void parse_command (struct document* doc) {
	struct charv* name = new_charv(10);
	struct macro* macro;
	
	document_fetchc (doc);
	if ( (doc->c < 65 || doc->c > 90) && (doc->c < 97 || doc->c > 122) && doc->c < 127) {
        struct charv* name = new_charv (1);
        charv_append (name, doc->c);
        charv_finalize (name);
        doc_putc (doc, doc->c);
        msg_log ("single-char macro", name->array);
        charv_free (name);
        return;
    } else charv_append (name, doc->c);
	
	while (document_fetchc (doc)) {
	    
		switch (doc->c) {
			case '{':
			case ' ':
			case '\t':
			case '\n':
				charv_finalize (name);
				macro = mstack_get_macro (doc->mstack, name->array);
				if (macro == 0) {
					unknown_command_exit (name->array);
				}
				if (macro->argc != 0) {
					struct charv** argv = get_statements (doc, macro->argc);
					char* output = macro->invoke (macro, argv);
					struct charv* ret = new_charv (10);
	                document_push_layer_command (doc, output, ret);
	                parse_main_loop (doc);
	                document_pop_layer (doc);
	                charv_finalize (ret);
	                doc_print (doc, ret->array);
					free (output);
					charv_free (ret);
					charv_array_free (argv, macro->argc);
				} else doc_print (doc, macro->invoke (macro, 0));
				msg_log ("macro", name->array);
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

