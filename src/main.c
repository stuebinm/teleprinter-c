#include <stdio.h>
#include <stdlib.h>

#include "document.h"
#include "parsing.h"


FILE* input;

int fetch_inputc () {
	return fgetc (input);
}


int main (int argc, char** argv) {

	if (argc != 2) {
		printf ("this program takes exactly one argument!\n");
		return 0;
	}
	
	
	input = fopen (argv[1], "r");
	
	if (input != 0) {
		
		struct document* doc = new_document ();
		doc->getc = &fetch_inputc;
		
		parse_main_loop (doc);
		
		fclose (input);
	}
	else {
		printf ("Could not open file %s.\n", argv[1]);
	}
	return 0;
}

