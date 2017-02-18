#include "charv.h"

#include <stdlib.h>

struct charv* new_charv (int l) {
	struct charv* ret = malloc (sizeof (struct charv));
	ret->array = malloc (l);
	ret->length = 0;
	ret->mem = l;
	return ret;
}

void charv_append (struct charv* charv, char c) {
	if (charv->length < charv->mem) {
		charv->array[charv->length] = c;
		charv->length++;
	} else if (charv->length == charv->mem) {
		charv->mem += 10;
		charv->array = realloc (charv->array, charv->mem);
		charv->array [charv->length] = c;
		charv->length += 1;
	}
}

void charv_finalize (struct charv* charv) {
	if (charv->length > charv->mem-1) {
		charv->array = realloc (charv->array, charv->length+1);
	}
	charv->array[charv->length] = 0; // terminating null byte for c string
	charv->mem = -1;
}

void charv_free (struct charv* charv) {
	free (charv->array);
	free (charv);
}
