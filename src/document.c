#include "document.h"

#include <stdio.h>
#include <stdlib.h>

#include "error.h"


void printc (struct layer* l, char c) {
	printf ("%c", c);
}

void printout (struct layer* l, char* str) {
	printf ("%s", str);
}

int fetch_from_file (struct layer* l) {
    FILE* input = (FILE*) l->indata;
    return fgetc (input);
}

struct document* new_document_from_file (FILE* input) {
	struct document* ret = malloc (sizeof (struct document));
	ret->top = malloc (sizeof (struct layer));
	
	ret->top->printc = &void_putc;
	ret->top->printf = &void_print;
	ret->top->fetchc = &fetch_from_file;
	ret->top->indata = input;
	ret->top->outdata = 0;
	ret->top->next = 0;
	ret->top->paragraph = "";
	ret->wordc = 0;
	ret->layerstack = 0;
	
	ret->mstack = new_macrostack ();
	ret->tagstack = new_tagstack ();
	document_fetchc (ret);
	
	return ret;
}

void free_document (struct document* doc) {
    struct layer* iter = doc->top;
    
    if (doc->top->next != 0) eof_error ();
    
    struct layer* next = iter;
    
    while (next != 0) {
        next = iter->next;
        free (iter);
        iter = next;
    }
    macrostack_free (doc->mstack);
    free_tagstack (doc->tagstack);
    free (doc);
}


void put_in_charv (struct layer* l, char c) {
    struct charv* strdoc = (struct charv*) l->outdata;
    charv_append (strdoc, c);
}

void print_in_charv (struct layer* l, char* str) {
    struct charv* strdoc = (struct charv*) l->outdata;
    char c;
    int i = 0;
    while ( (c = str[i]) != '\0') {
        charv_append (strdoc, c);
        i++;
    }
}

int fetch_from_base (struct layer* l) {
    int c = l->next->fetchc (l->next);
    l->doc->c = c;
	//if (c == '}') c = EOF;
	l->c = c;
	l->next->c = c;
	return c;
}

struct strin_data {
    char* string;
    int i;
};

int fetch_from_string (struct layer* l) {
    struct strin_data* indata= (struct strin_data*) l->indata;
    char c = indata->string [indata->i];
    if (c != '\0') indata->i += 1;
    else return EOF;
    return c;
}

struct layer* document_push_layer (struct document* doc) {
    struct layer* new = malloc (sizeof (struct layer));
    new->next = doc->top;
	new->doc = doc;
	new->c = doc->c;
	doc->top = new;
	return new;
}

void document_push_layer_str (struct document* doc, struct charv* out) {
    struct layer* new = document_push_layer (doc);
	
	new->printc = &put_in_charv;
	new->printf = &print_in_charv;
	new->fetchc = &fetch_from_base;
	new->indata = 0;
	new->outdata = 0;
	new->paragraph = "\n</p>\n<p align=\"justify\">\n";
	new->outdata = out;
}

void document_push_layer_command (struct document* doc, char* in, struct charv* out) {
    
    struct layer* new = document_push_layer (doc);

    // input
	struct strin_data* indata = malloc (sizeof (struct strin_data));
	indata->string = in;
	indata->i = 0;
	new->indata = indata;
	new->fetchc = &fetch_from_string;
	// fetch first character of input
	document_fetchc (doc);
	
	// output
	new->outdata = out;
	new->printc = &put_in_charv;
	new->printf = &print_in_charv;

    // TODO: this is a hack
	new->paragraph = "\n</p>\n<p align=\"justify\">\n";
}

void document_push_layer_env (struct document* doc, char* name) {
    
    struct layer* new = document_push_layer (doc);
    
    // take the base output functions (before this env, there's only a dummy function)
	new->printc = doc->printc_base;
	new->printf = doc->printf_base;
	// input remains unchanged
	new->fetchc = &fetch_from_base;
	
	new->indata = 0;
	new->outdata = 0;
	new->paragraph = "\n</p>\n<p align=\"justify\">\n";
}

void document_push_layer_doc (struct document* doc, FILE* file) {

    struct layer* new = document_push_layer (doc);
    
    // output doesn't change
	new->outdata = 0;
	new->printc = new->next->printc;
	new->printf = new->next->printf;
	
	// fetch from given file
	new->indata = file;
	new->fetchc = &fetch_from_file;

	new->paragraph = new->next->paragraph;
}


void document_pop_layer (struct document* doc) {
    struct layer* old = doc->top;
    doc->top = old->next;
    doc->c = doc->top->c;
    if (doc->top == 0) {
        parse_error ();
    }
    free (old);
}

void document_pop_layer_command (struct document* doc) {
    free (doc->top->indata);
    document_pop_layer (doc);
}


void document_push_scope (struct document* doc) {
    mstack_push_level (doc->mstack);
}
void document_pop_scope (struct document* doc) {
    mstack_pop_level (doc->mstack);
}


int document_fetchc (struct document* doc) {
    doc->top->c = doc->top->fetchc (doc->top);
    doc->c = doc->top->c;
    return doc->c;
}


void doc_putc (struct document* doc, char c) {
    doc->top->printc (doc->top, c);
}
void doc_print (struct document* doc, char* str) {
    doc->top->printf (doc->top, str);
}

void void_putc (struct layer* l, char c) {
    switch (c) {
        case '\n':
        case '\t':
        case ' ':
            return;
        default:
            non_doc_error ();
    }
}

void void_print (struct layer* l, char* msg) {
    if (msg[0] != 0)
    non_doc_error ();
}

