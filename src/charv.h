#ifndef CHARV_H
#define CHARV_H


struct charv {
	char* array;
	int length;
	int mem;
};

struct charv* new_charv (int length);

void charv_append (struct charv* charv, char c);
void charv_append_array (struct charv* charv, char* a);
void charv_finalize (struct charv* charv);

char* charv_isolate (struct charv* charv);

void charv_free (struct charv* charv);
void charv_array_free (struct charv** array, int l);

#endif
