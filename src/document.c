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
	
	ret->top->printc = &printc;
	ret->top->printf = &printout;
	ret->top->fetchc = &fetch_from_file;
	ret->top->indata = input;
	ret->top->next = 0;
	
	ret->mstack = new_macrostack ();
	document_fetchc (ret);
	
	return ret;
}

void free_document (struct document* doc) {
    struct layer* iter = doc->top;
    
    struct layer* next;
    
    while (next != 0) {
        next = iter->next;
        free (iter);
    }
    macrostack_free (doc->mstack);
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
    if (c == '\0') return EOF;
    indata->i += 1;
    return c;
}

void document_push_layer_str (struct document* doc, struct charv* out) {
    
    struct layer* new = malloc (sizeof (struct layer));
    
	new->printc = &put_in_charv;
	new->printf = &print_in_charv;
	
	new->fetchc = &fetch_from_base;
	new->doc = doc;
	
	new->outdata = out;
	new->next = doc->top;
	doc->top = new;
	new->c = doc->c;
	
	mstack_push_level (doc->mstack);
	
}

void document_push_layer_command (struct document* doc, char* in, struct charv* out) {
    
    struct layer* new = malloc (sizeof (struct layer));
    
	new->printc = &put_in_charv;
	new->printf = &print_in_charv;
	
	new->fetchc = &fetch_from_string;
	struct strin_data* indata = malloc (sizeof (struct strin_data));
	indata->string = in;
	indata->i = 1;
	new->indata = indata;
	new->doc = doc;
	
	new->outdata = out;
	new->next = doc->top;
	doc->top = new;
	new->c = in[0];
	doc->c = new->c;
	
	mstack_push_level (doc->mstack);
}

void document_pop_layer (struct document* doc) {
    struct layer* old = doc->top;
    doc->top = old->next;
    doc->c = doc->top->c;
    if (doc->top == 0) {
        parse_error ();
    }
    free (old);
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
