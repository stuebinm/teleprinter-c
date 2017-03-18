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
    ret->htable = calloc (ret->size, sizeof (struct macro*));
    
    // these are the baseline commands used to output all html tags.
    mstack_set_macro (ret, "otag", 1, &custom_method, "<#1>", true);
    mstack_set_macro (ret, "ctag", 1, &custom_method, "</#1>", true);
    mstack_set_macro (ret, "tag", 2, &custom_method, "\\otag{#1}#2\\ctag{#1}", false);
    
     // a few basic commands for testing
    //mstack_set_macro (ret, "emph", 1, &custom_method, "\\tag{em}{#1}", false);
    mstack_set_macro (ret, "chapter*", 1, &custom_method, "\\tag{h2}{#1}", false);
    mstack_set_macro (ret, "part", 1, &custom_method, "\\tag{h1}{#1}", false);
    mstack_set_macro (ret, "chbegin", 2, &custom_method, "\\tag{h4}{#1}\\tag{b}{#2}", false);
    mstack_set_macro (ret, "enquote", 1, &custom_method, "“#1”", false);
    
    mstack_set_macro (ret, "include", 1, &include_doc_method, 0, true);
    
     // the all-important \newcommand
    mstack_set_macro (ret, "newcommand", 2, &newcommand_method, 0, false);
    
     // env testing
    mstack_set_macro (ret, "begin", 0, &begin_env_method, 0, false);
    mstack_set_macro (ret, "end", 0, &end_env_method, 0, false);
    
    
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
                free (iter);
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


void mstack_set_macro (struct mstack* mstack, char* name, int argc, macro_method* method, char* data, bool raw) {
    int key = hash (name, mstack->size);
    
    struct macro* new = malloc (sizeof (struct macro));
    new->name = name;
    new->argc = argc;
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
    msg_log ("macro now out of scope", m->name);
    free (m->name);
    if (m->data != 0) free (m->data);
    free (m);
}
