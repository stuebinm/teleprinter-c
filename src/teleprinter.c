#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

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


void print_document (FILE* l_input, FILE* l_output, FILE* log, FILE* err) {
    
    FILE* logstacked = LOGFILE;
    FILE* errstacked = ERRFILE;
    
    LOGFILE = log;
    ERRFILE = err;
    
    input = l_input;
    output = l_output;
    
    if (LOGFILE == stdout) {
        struct winsize w;
        ioctl (0, TIOCGWINSZ, &w);
        LOGWIDTH = w.ws_col;
    } else {
        LOGWIDTH = 100;
    }

	if (input != 0) {
		
		struct document* doc = new_document_from_file (input);

		if (output == 0) error_exit (UNKNOWN_FILE_ERROR, "File Error");
		
		doc->printc_base = &put_outputc;
		doc->printf_base = &put_printout;
		
		parse_main_loop (doc);
		free_document (doc);
		fclose (input);
		fclose (output);
		
	}
	else {
		error_exit (UNKNOWN_FILE_ERROR, "Could not open file");
	}
	
	LOGFILE = logstacked;
	ERRFILE = errstacked;
}

