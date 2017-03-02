#include "assembly.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

char* newcommand_method (struct document* doc, struct macro* macro, struct charv** argv) {
    msg_log ("new macro", argv[0]->array);
    mstack_set_macro (doc->mstack, argv[0]->array, 1, &custom_method, argv[1]->array);
    return calloc (1, sizeof (char));
}

char* custom_method (struct document* doc, struct macro* macro, struct charv** argv) {
    struct charv* ret = new_charv (argv[0]->length);
    char c = macro->data[0];
    int i = 0;
    while ( (c = macro->data[i]) != '\0') {
        //printf ("%c", c);
        if (c == '#') {
            
            switch (macro->data[i+1]) {
                case '1':
                    charv_append_array (ret, argv[0]->array);
                    break;
                case '2':
                    charv_append_array (ret, argv[1]->array);
                    break;
                default:
                    charv_append (ret, argv[0]->array[i]);
                    goto big_loop;
            }
            i += 2;
            continue;
        }
        big_loop:
        charv_append (ret, c);
        i++;
    }
    charv_finalize (ret);
	charv_array_free (argv, macro->argc);
    return charv_isolate (ret);
}

char* begin_env_method (struct document* doc, struct macro* macro, struct charv** argv) {
    
    document_push_layer_env (doc, "document");
    msg_log ("entering env", "document");
	return calloc (1, sizeof (char));
}

char* end_env_method (struct document* doc, struct macro* macro, struct charv** argv) {
    if (doc->top->next == 0) {
        leaving_env_error ("document");
    }
    document_pop_layer (doc);
    msg_log ("leaving env", "document");
    return calloc (1, sizeof (char));
}

