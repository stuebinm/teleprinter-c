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


//void print_document (FILE* l_input, FILE* l_output, FILE* log, FILE* err) {
void print_document (struct charv* prefix, struct charv* workingdir, FILE* log, FILE* err) {
    
    FILE* logstacked = LOGFILE;
    FILE* errstacked = ERRFILE;
    
    LOGFILE = log;
    ERRFILE = err;
    
    /*input = l_input;
    output = l_output;
    */
    
    struct charv* inputname = new_charv (prefix->length);
    charv_append_array (inputname, prefix->array);
    charv_append_array (inputname, ".tex");
    charv_finalize (inputname);
    input = fopen (charv_isolate (inputname), "r");
    
    
    struct charv* outputname = new_charv (prefix->length);
    charv_append_array (outputname, prefix->array);
    charv_append_array (outputname, ".html");
    charv_finalize (outputname);
    output = fopen (charv_isolate (outputname), "w");

    
    if (LOGFILE == stderr) {
        struct winsize w;
        ioctl (0, TIOCGWINSZ, &w);
        LOGWIDTH = w.ws_col;
    } else {
        LOGWIDTH = DEFAULT_FILEWIDTH;
    }

	if (input != 0) {
		
		struct document* doc = new_document_from_file (input);
		doc->prefix = prefix;
		doc->localdir = workingdir;

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

