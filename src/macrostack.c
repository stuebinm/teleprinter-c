#include "macrostack.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "assembly.h"
#include "error.h"

void free_macro (struct macro* m);


struct mstack* new_macrostack () {
    struct mstack* ret = malloc (sizeof (struct mstack));
    
    ret->size = 20;
    ret->top = 0;
    ret->htable = calloc (ret->size, sizeof (struct macro*));
    
    // these are the baseline commands used to output all html tags.
    mstack_set_macro (ret, "otag", 1, 0, &custom_method, "<#1>", true);
    mstack_set_macro (ret, "ctag", 1, 0, &custom_method, "</#1>", true);
    mstack_set_macro (ret, "tag", 3, 1, &custom_method, "\\otag{#2 #1}#3\\ctag{#2}", false);
    
    // control commands
    mstack_set_macro (ret, "message", 1, 0, &message_method, 0, true);
    mstack_set_macro (ret, "error", 1, 0, &error_method, 0, true);
    mstack_set_macro (ret, "include", 1, 0, &include_doc_method, 0, true);
    mstack_set_macro (ret, "ifthenelse", 3, 0, &ifthenelse_method, 0, false);
    
    
    // boolean values
    mstack_set_macro (ret, "no", 0, 0, &custom_method, " ", true);
    mstack_set_macro (ret, "yes", 0, 0, &custom_method, "yes", true);
    
     // the all-important \newcommand
    mstack_set_macro (ret, "newcommand", 4, 2, &newcommand_method, 0, false);
    
     // a few basic commands for testing
    //mstack_set_macro (ret, "emph", 1, &custom_method, "\\tag{em}{#1}", false);
    mstack_set_macro (ret, "chapter*", 1, 0, &custom_method, "\\tag{h2}{#1}", false);
    mstack_set_macro (ret, "part", 1, 0, &custom_method, "\\tag{h1}{#1}", false);
    mstack_set_macro (ret, "chbegin", 2, 0, &custom_method, "\\tag{h4}{#1}\\tag{b}{#2}", false);
    
     // env testing
    mstack_set_macro (ret, "begin", 0, 0, &begin_env_method, 0, false);
    mstack_set_macro (ret, "end", 0, 0, &end_env_method, 0, false);
    
    ret->top = 1;
    return ret;
}

void macrostack_free (struct mstack* mstack) {
    for (int i = 0; i<mstack->size; i++) {
        if (mstack->htable[i] != 0) {
            struct macro* iter = mstack->htable[i];
            struct macro* old = iter->next;
            
            while (iter != 0) {
                old = iter->next;
                /*  at this point all but the built-in assembly commands are already freed,
                    so there's no need call free_macro(),
                    wich assumes that the macro's name was allocated on the stack.*/
                free_macro (iter);
                iter = old;
            }
        }
    }
    free (mstack->htable);
    free (mstack);
}

int hash (char* key, int size) {
    unsigned long int ret = 0;
    
    int i = 0;
    int l = strlen (key);
    while (ret < ULONG_MAX && i<l) {
        ret = ret << 8;
        ret += key[i];
        i++;
    }
    return ret % size;
}

void mstack_push_level (struct mstack* mstack) {
    mstack->top += 1;
}

void mstack_pop_level (struct mstack* mstack) {
    for (int i = 0; i < mstack->size; i++) {
        if (mstack->htable[i] == 0) continue;
        struct macro* iter = mstack->htable[i];
        
        while (iter != 0 && iter->story == mstack->top) {
            mstack->htable[i] = iter->next;
            printf ("freeing:  %s\n", iter->name);
            free_macro (iter);
            iter = mstack->htable[i];
        }
        
    }
    mstack->top -= 1;
}


void mstack_set_macro (struct mstack* mstack, char* name, int argc, int opc, macro_method* method, char* data, bool raw) {
    int key = hash (name, mstack->size);
    
    struct macro* new = malloc (sizeof (struct macro));
    new->name = name;
    new->argc = argc;
    new->opc = opc;
    new->invoke = method;
    new->story = mstack->top;
    new->next = mstack->htable [key];
    new->data = data;
    new->raw = raw;
    mstack->htable[key] = new;
    
}

struct macro* mstack_get_macro (struct mstack* mstack, char* name) {
    int key = hash (name, mstack->size);
    
    struct macro* ret = mstack->htable [key];
    if (ret == 0) return ret;
    while (strcmp (ret->name, name)) {
        ret = ret->next;
        if (ret == 0) return ret;
    }
    
    return ret;
}

void free_macro (struct macro* m) {
    if (m->story) {
        msg_log ("macro now out of scope", m->name);
        // based on the assumption that m at this point is sure to be one that's been allocated by
        // the user (i.e. via \newcommand) --- the built-in ones have story = 0. In this case, the
        // name pointer actuall points at the second character of the chararray (at the first is the
        // mandatory '\'), so it's necessary to subract 1 from it.
        free (m->name-1);
        if (m->data != 0) free (m->data);
    }
    free (m);
}
