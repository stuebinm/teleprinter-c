#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <stdio.h>

#include "charv.h"
#include "macrostack.h"

struct document;
struct layer;

typedef void (stream_putc) (struct layer* l, char c);
typedef void (stream_printf) (struct layer* l, char* format);
typedef int (stream_fetchc) (struct layer* l);

struct document {
	struct layer* top;
	struct mstack* mstack;
	int c;
	int wordc;
	stream_putc* printc_base;
	stream_printf* printf_base;
};

struct layer {
    struct layer* next;
    void* indata;
    void* outdata;
	stream_putc* printc;
	stream_printf* printf;
	stream_fetchc* fetchc;
	int c;
	struct document* doc;
	char* paragraph;
};


struct document* new_document_from_file (FILE* input);
void free_document (struct document* doc);

void document_push_layer_str (struct document* doc, struct charv* out);
void document_push_layer_command (struct document* doc, char* in, struct charv* out);
void document_push_layer_env (struct document* doc, char* name);
void document_push_layer_doc (struct document* doc, FILE* file);
void document_pop_layer (struct document* doc);

int document_fetchc (struct document* doc);

void document_push_scope (struct document* doc);
void document_pop_scope (struct document* doc);

void doc_putc (struct document* doc, char c);
void doc_print (struct document* doc, char* str);

void void_putc (struct layer* l, char c);
void void_print (struct layer* l, char* msg);

#endif
