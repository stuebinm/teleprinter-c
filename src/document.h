#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "layer.h"

typedef void (stream_putc) (char c);
typedef void (stream_printf) (char* format);
typedef int (stream_getc) ();

struct document {
	stream_putc* printc;
	stream_printf* printf;
	stream_getc* getc;
	struct layer* base;
	int c;
};

struct document* new_document ();


#endif
