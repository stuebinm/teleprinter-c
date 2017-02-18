#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "layer.h"

typedef void (stream_putc) (char c);
typedef int (stream_getc) ();

struct document {
	stream_putc* printc;
	stream_getc* getc;
	struct layer* base;
	char c;
};

struct document* new_document ();


#endif
