#ifndef MACROSTACK_H
#define MACROSTACK_H

#include <stdbool.h>

#include "charv.h"
#include "document.h"

struct document;

struct macro;
struct mstack;

typedef char* (macro_method) (struct document* doc, struct macro* macro, struct charv** argv);

struct macro {
    char* name;
    macro_method* invoke;
    int argc;
    char* data;
    struct macro* next;
    int story;
    bool raw;
};

struct mstack {
    struct macro** htable;
    int size;
    int top;
};

struct mstack* new_macrostack ();
void macrostack_free (struct mstack* mstack);

void mstack_push_level (struct mstack* stack);
void mstack_pop_level (struct mstack* stack);

void mstack_set_macro (struct mstack*, char* name, int argc, macro_method* method, char* data, bool raw);

struct macro* mstack_get_macro (struct mstack*, char* name);



#endif
