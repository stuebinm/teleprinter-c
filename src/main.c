#include <stdio.h>
#include <stdlib.h>

#include "document.h"
#include "parsing.h"
#include "commands.h"
#include "charv.h"
#include "error.h"


FILE* input;
FILE* output;

int fetch_inputc () {
	return fgetc (input);
}

void put_outputc (char out) {
	fputc (out, output);
}

void put_printout (char* format) {
	fprintf (output, "%s", format);
}


int main (int argc, char** argv) {

	if (argc < 2 || argc > 3) {
		printf ("this program takes one or two arguments!\n");
		return 0;
	}
	
	
	input = fopen (argv[1], "r");
	
	if (input != 0) {
		
		struct document* doc = new_document ();
		doc->getc = &fetch_inputc;
		commands_init ();
		
		if (argc == 3) {
			output = fopen (argv[2], "w");
			if (output == 0) error_exit (1, "File Error");
			doc->printc = &put_outputc;
			doc->printf = &put_printout;
			
		}
		
		parse_main_loop (doc);
		
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

