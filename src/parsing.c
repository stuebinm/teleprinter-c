#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "macrostack.h"
#include "error.h"

void parse_main_loop (struct document* doc) {
	skip_whitespace (doc);
	doc->layerstack++;
	do{
	    main_loop_start:
		if (skip_whitespace (doc)) {
			if (doc->wordc > 60) {
				doc_putc (doc, '\n');
				doc->wordc = 0;
			} else doc_putc (doc, ' ');
		}
		switch (doc->c) {
		     // code for entering scopes (floating ones, obviously; others are handled by macros)
			case '{': ;
				struct charv* word = parse_statement (doc);
				msg_log ("floating word", word->array);
				doc_print (doc, word->array);
				charv_free (word);
				break;
			 // code for leaving scopes; exit with error if toplevel.
			case '}':
			    if (doc->layerstack == 0) parse_error();
			    goto main_end;
			 // code for EOF (exit with error unless toplevel)
		    case EOF:
		        /*if (d != 1)
		            eof_error ();
		        */goto main_end;
	         // code for macros.
			case '\\':
				parse_command (doc);
				goto main_loop_start;
				break;
		     // code for long dashes.
			case '-':
			    if (document_fetchc (doc) == '-') {
			        if (document_fetchc (doc) == '-') {
			            msg_log ("character identity", "em dash");
			            doc_print (doc, "&mdash;");
			            break;
		            } else {
			            msg_log ("character identity", "en dash");
			            doc_print (doc, "&ndash;");
			            goto main_loop_start;
			        }
			    }
			    else doc_putc (doc, '-');
			    goto main_loop_start;
		     // code for xml character entities.
		    case '\'':
		        doc_print (doc, "’");
		        break;
	        case '<':
	            doc_print (doc, "&lt;");
	            break;
	        case '>':
	            doc_print (doc, "&gt;");
	            break;
	        case '&':
	            doc_print (doc, "&amp;");
	            break;
	        case '"':
	            doc_print (doc, "&quot;");
	            break;
             // code for paragraphs
			case '\n':
			    document_fetchc (doc);
			    if (doc->c != '\n') goto main_loop_start;
			    doc_print (doc, doc->top->paragraph);//"\n</p>\n<p align=\"justify\">\n");
		     // default to copy things (html will do the rest)
			default:
				doc_putc (doc, (char) doc->c);
				break;
		}
		doc->wordc++;
	} while (document_fetchc (doc));
	main_end:
	doc->layerstack--;
}

/**
 * parse_statment:
 * parses the next statement in its own scope.
 * @doc: the document.
 *
 * Returns: the generated html source.
*/
struct charv* parse_statement (struct document* doc) {
	struct charv* ret = new_charv (10);
	document_fetchc (doc);
	
	document_push_scope (doc);
	document_push_layer_str (doc, ret);
	
	parse_main_loop (doc);
	
	document_pop_layer (doc);
	document_pop_scope (doc);
	
	charv_finalize (ret);
	return ret;
}

/**
 * skip_comment:
 * Skips a comment.
 *
*/
void skip_comment (struct document* doc) {
	while (document_fetchc (doc)) {
		if (doc->c == '\n') return;
		if (doc->c == EOF) parse_error ();
	}
}

/**
 * skip_whitespace:
 * skips any unimportant text following the cursor's position (whitespace, comments …)
 * @doc: the document in question.
 * 
 * Returns: if anything was skipped.
 *
*/
int skip_whitespace (struct document* doc) {
	int s = 0;
	do {
	    switch (doc->c) {
		    case '%':
			    skip_comment (doc);
			    s = 1;
			    continue;
		    case ' ':
		    case '\t':
			    s = 1;
			    continue;
		    default:
			    return s;
	    }
	} while (document_fetchc (doc));
}

/**
 * get_statements:
 * gets the next @number statements, but doesn't parse them (that's for #parse_command).
 * @doc: the document to work on.
 * @number: the number of statements requested.
 *
 * Returns: a charv-array containing @number elements.
 *
*/
struct charv** get_statements (struct document* doc, int number, int optional, char* name) {
    if (number == 0) return 0;
	struct charv** ret = malloc (number * sizeof (struct charv));
	for (int i = 0; i<number; i++) {
		start_get_loop:
		do {
		    int d;
			switch (doc->c) {
			    case '[':
			        if (i>=optional) {
			            command_error ("Too many optional arguments for macro ", name, "!");
			        }
			        // fetch the i-th statement.
					ret[i] = new_charv (10);
					d = 1;
					while (document_fetchc (doc)) {
					    switch (doc->c) {
					        case '[':
					            d += 1;
					            charv_append (ret[i], doc->c);
					            break;
				            case ']':
				                d -= 1;
				                if (d == 0) {
				                    charv_finalize (ret[i]);
				                    document_fetchc (doc);
				                    goto cont;
				                }
				                charv_append (ret[i], doc->c);
				                break;
			                case EOF:
			                    eof_error ();
			                default:
			                    charv_append (ret[i], doc->c);
					    }
					}
					goto cont;
				case '{':
				    if (i<optional) {
				        while (i<optional) {
				            ret[i] = new_charv (10);
				            charv_finalize (ret[i]);
				            i++;
				        }
				    }
				    if (optional == number) {
				        while (i<optional) {
				            ret[i] = new_charv (10);
				            charv_finalize (ret[i]);
				            i++;
				        }
				        goto cont;
				    }
				    
				    // fetch the i-th statement.
					ret[i] = new_charv (10);
					d = 1;
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
			                case EOF:
			                    eof_error ();
			                default:
			                    charv_append (ret[i], doc->c);
					    }
					}
					goto cont;
				default:
				    if (!skip_whitespace (doc)) {
				        command_error ("too few arguments for macro ", name, "!");
				    }
				    goto start_get_loop;
			}
		}
		while (document_fetchc (doc));
	cont: ;
	}
	return ret;
}


/**
 * parse_command:
 * assumes there is a command next, fetches and executes it.
 * @doc: the document in question.
 *
*/
void parse_command (struct document* doc) {
	struct charv* name = new_charv(10);
	struct macro* macro;

	 // in case the first character isn't a letter, do a TeX-style char conversion.
	 // TODO!
	document_fetchc (doc);
	if ( (doc->c < 65 || doc->c > 90) && (doc->c < 97 || doc->c > 122) || doc->c > 127) {
         //these three lines aren't really necessary, but serve to print out a nice little msg …
        charv_append (name, doc->c);
        charv_finalize (name);
        msg_log ("escape sequence", name->array);
        charv_free (name);
        doc_putc (doc, doc->c);
        document_fetchc (doc);
        return;
    } else charv_append (name, doc->c);
	
	while (document_fetchc (doc)) {
	    
		switch (doc->c) {
			case '{':
			case '[':
			case ' ':
			case '\t':
			case '\n':
				charv_finalize (name);
				macro = mstack_get_macro (doc->mstack, name->array);
				
				 // safeguard; in case the user gives a non-existent one the program will exit here.
				if (macro == 0) {
					command_error ("Unknown macro ", name->array, "!");
				}
				
				 // fetch statements (will return 0 in case none are requested)
				struct charv** argv = get_statements (doc, macro->argc, macro->opc, macro->name);
				 // generate markup.
				char* output = macro->invoke (doc, macro, argv);
				if (macro->raw) {
				    doc_print (doc, output);
				}
				else {
			         // parse said markup in its own scope.
			        struct charv* ret = new_charv (10);
			        
			        document_push_scope (doc);
                    document_push_layer_command (doc, output, ret);
                    parse_main_loop (doc);
                    document_pop_layer_command (doc);
                    document_pop_scope (doc);
                    
                    charv_finalize (ret);
		             // be colloquial
                    doc_print (doc, ret->array);
                     // free memory
				    charv_free (ret);
                }
                // prints a debug-message if the layer stack has fewer than two entries.
		        if (doc->layerstack<2) msg_log ("macro", name->array);
				
				free (output);
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

