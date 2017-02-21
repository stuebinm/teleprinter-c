#include "document.h"

#include <stdio.h>
#include <stdlib.h>


void printc (char c) {
	printf ("%c", c);
}

void printout (char* str) {
	printf ("%s", str);
}

struct document* new_document () {
	struct document* ret = malloc (sizeof (struct document));
	
	ret->printc = &printc;
	ret->printf = &printout;
	
	struct layer* base = malloc (sizeof (struct layer));
	ret->base = base;
	base->doc = ret;
	
	return ret;
}

