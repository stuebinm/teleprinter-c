#include "document.h"

#include <stdio.h>
#include <stdlib.h>


void printc (char c) {
	printf ("%c", c);
}

struct document* new_document () {
	struct document* ret = malloc (sizeof (struct document));
	
	ret->printc = &printc;
	
	struct layer* base = malloc (sizeof (struct layer));
	ret->base = base;
	base->doc = ret;
	
	return ret;
}

