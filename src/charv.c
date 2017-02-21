#include "charv.h"

#include <stdlib.h>
#include <stdio.h>

struct charv* new_charv (int l) {
	struct charv* ret = malloc (sizeof (struct charv));
	ret->array = malloc (l);
	ret->length = 1;
	ret->mem = l;
	return ret;
}

void charv_append (struct charv* charv, char c) {
	if (charv->length+1 < charv->mem) {
		charv->array[charv->length-1] = c;
		charv->length++;
	} else {
		charv->mem += 10;
		charv->array = realloc (charv->array, charv->mem);
		charv->array [charv->length-1] = c;
		charv->length += 1;
	}
}

void charv_finalize (struct charv* charv) {
	if (charv->length > charv->mem-1) {
		charv->array = realloc (charv->array, charv->length+1);
	}
	charv->array[charv->length-1] = 0; // terminating null byte for c string
	charv->mem = -1;
}

void charv_free (struct charv* charv) {
	free (charv->array);
	free (charv);
}

void charv_array_free (struct charv** array, int l) {
	for (int i = 0; i<l; i++) {
		free (array[i]->array);
		free (array[i]);
	}
}
