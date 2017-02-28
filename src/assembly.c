#include "assembly.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* newcommand_method (struct mstack* mstack, struct macro* macro, struct charv** argv) {
    mstack_set_macro (mstack, argv[0]->array, 1, &custom_method, argv[1]->array);
    return calloc (1, sizeof (char));
}

char* custom_method (struct mstack* mstack, struct macro* macro, struct charv** argv) {
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
