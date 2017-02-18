#ifndef CHARV_H
#define CHARV_H


struct charv {
	char* array;
	int length;
	int mem;
};

struct charv* new_charv (int length);

void charv_append (struct charv* charv, char c);
void charv_finalize (struct charv* charv);

void charv_free (struct charv* charv);

#endif
