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
    char* name; // name of the macro (as in "\name")
    macro_method* invoke; // method to execute when the macro is used
    int argc; // total number of arguments
    int opc; // number of args that are optional (must be less than argc!)
    char* data; // pointer to additional data
    bool raw; // if the output is to be interpreted or not
    
     // stuff for internal memory management
    struct macro* next;
    int story; // scope this is in
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

void mstack_set_macro (struct mstack*, char* name, int argc, int opc, macro_method* method, char* data, bool raw);

struct macro* mstack_get_macro (struct mstack*, char* name);



#endif
