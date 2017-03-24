#include "assembly.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

char* newcommand_method (struct document* doc, struct macro* macro, struct charv** argv) {
    msg_log ("new macro", argv[2]->array);
    int argc = 0;
    int opc = 0;
    if (argv[0]->array[0] != '\0') {
        argc = argv[0]->array[0] - 48; // ascii decoding
    }
    if (argv[1]->array[0] != '\0') {
        opc = argv[1]->array[0] - 48; // ascii decoding
    }
    if (opc > argc) {
        newcommand_error ("number of optional arguments must be lower than total argument count.");
    }
    if (argv[2]->array[0] != '\\') newcommand_error ("macro names must start with \"\\\"!");
    int i = 1;
    char c;
    while ( (c = argv[2]->array[i]) != '\0') {
        if (c == ' ' || c == '\t' || c == '\n') 
            newcommand_error ("Whitespace is not allowed in macro names.");
        if (c == '{' || c == '[' || c == ']' || c == '[')
            newcommand_error ("Control characters are not allowed in macro names.");
        i += 1;
    }
    
     //                                                   ↓ this gives the actual name (minus the '\'). It does make freeing stuff somewhat ugly; see the comment in free_macro() for that
    mstack_set_macro (doc->mstack, charv_isolate (argv[2])+1, argc, opc, &custom_method, charv_isolate(argv[3]), false);
    
    charv_free (argv[0]);
    charv_free (argv[1]);
    free (argv);
    
    return calloc (1, sizeof (char));
}

char* custom_method (struct document* doc, struct macro* macro, struct charv** argv) {
    struct charv* ret = new_charv (strlen(macro->data));
    char c = macro->data[0];
    int i = 0;
    while ( (c = macro->data[i]) != '\0') {

        if (c == '#') {
            i += 1;
            if (macro->data[i] > 48 && macro->data[i] < 58) {
                int n = macro->data [i] - 49; // - 48 (for ascii decoding) -1 (for array count)
                if (n>=macro->argc) //command_error ("this is an error: ", macro->name, "!");
                {
                    charv_append (ret, '#');
                    charv_append (ret, macro->data[i]);
                    printf ("%s", ret->array);
                    i += 1;
                    continue;
                }
                charv_append_array (ret, argv[n]->array);
            }
            i += 1;
            continue;
        }
        charv_append (ret, c);
        i += 1;
    }
    charv_finalize (ret);
	charv_array_free (argv, macro->argc);
    return charv_isolate (ret);
}


char* include_doc_method (struct document* doc, struct macro* macro, struct charv** argv) {
    
    
    FILE* sinput = fopen (argv[0]->array, "r");
    if (sinput == 0) printf ("file error!\n");
    
    document_push_layer_doc (doc, sinput);
    parse_main_loop (doc);
    document_pop_layer (doc);
    
    fclose (sinput);
    
    charv_array_free (argv, macro->argc);
    return calloc (1, sizeof (char));
}



char* begin_env_method (struct document* doc, struct macro* macro, struct charv** argv) {
    document_push_layer_env (doc, "document");
    msg_log ("entering env", "document");
    doc_print (doc, "<html><head><meta charset=\"utf8\"></head><body>");

	return calloc (1, sizeof (char));
}

char* end_env_method (struct document* doc, struct macro* macro, struct charv** argv) {

    if (doc->top->next == 0) {
        leaving_env_error ("document");
    }
    doc_print (doc, "</body></html>");
    document_pop_layer (doc);
    
    msg_log ("leaving env", "document");
    return calloc (1, sizeof (char));
}

