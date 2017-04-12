#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
    
    struct charv* prefix = new_charv (10);
    char c;
    int i = 0;
    while ( (c = argv[1][i]) != '.') {
        charv_append (prefix, c);
        i += 1;
    }
    charv_finalize (prefix);
    
    struct charv* workingdir = new_charv (prefix->length);
    struct charv* shortdir = new_charv (10);
    i = 0;
    while ( (c = prefix->array[i]) != '\0') {
        charv_append (shortdir, c);
        if (c == '/') {
            charv_append_array (workingdir, charv_isolate (shortdir));
            shortdir = new_charv (10);
        }
        i++;
    }
    charv_finalize (workingdir);
    charv_finalize (shortdir);
    charv_free (shortdir);
    
    fprintf (stderr, "workingdir: '%s'", workingdir->array);
    
    FILE* logfile;
    if (argc >= 4) logfile = fopen ("printer.log", "w");
    else logfile = stderr;

    print_document (prefix, workingdir, logfile, stderr);
    
    msg_log ("printed document", argv[1]);
    msg_simple ("\n");
	
	return 0;
}

