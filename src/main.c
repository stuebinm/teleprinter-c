#include <stdio.h>
#include <stdlib.h>

#include "document.h"
#include "parsing.h"
#include "charv.h"
#include "error.h"


FILE* input;
FILE* output;


void put_outputc (struct layer* l, char out) {
	fputc (out, output);
}

void put_printout (struct layer* l, char* format) {
	fprintf (output, "%s", format);
}


int main (int argc, char** argv) {

	if (argc < 2 || argc > 3) {
		printf ("this program takes one or two arguments!\n");
		return 0;
	}

	
    input = fopen (argv[1], "r");

	if (input != 0) {
		
		struct document* doc = new_document_from_file (input);

		if (argc == 3) {
			output = fopen (argv[2], "w");
			
        } else { // simple code to construct a fallback output name in case none was given.
            struct charv* outname = new_charv (10);
            char c;
            int i = 0;
            while ( (c = argv[1][i]) != '.') {
                charv_append (outname, c);
                i++;
            }
            charv_append_array (outname, ".html");
            charv_finalize (outname);
			output = fopen (outname->array, "w");
			charv_free (outname);
        }
		if (output == 0) error_exit (1, "File Error");
		doc->top->printc = &put_outputc;
		doc->top->printf = &put_printout;
		
		parse_main_loop (doc);
		printf ("\n");
		free_document (doc);
		fclose (input);
		if (argc == 3) {
			fclose (output);
		}
	}
	else {
		printf ("Could not open file %s.\n", argv[1]);
	}
	
	return 0;
}

