#include <stdio.h>

#include "teleprinter.h"
#include "charv.h"
#include "error.h"

int main (int argc, char** argv) {

    ERRFILE = stderr;
    LOGFILE = stdout;

	if (argc > 4) {
		error_exit (1, "This program takes at most three arguments!");
	} else if (argc < 2) {
	    error_exit (1, "This programm takes at least one argument!");
	}
    
    FILE* input = fopen (argv[1], "r");
    FILE* output;
    if (argc >= 3) {
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
    
    FILE* logfile;
    if (argc >= 4) logfile = fopen ("printer.log", "w");
    else logfile = stderr;

    print_document (input, output, logfile, stderr);
    msg_log ("printed document", argv[1]);
    msg_simple ("\n");
	
	return 0;
}

