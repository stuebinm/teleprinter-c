#ifndef TAGSTACK_H
#define TAGSTACK_H

#include <stdbool.h>


struct tagstack {
    char* data;
    struct tagstack* next;
};


struct tagstack* new_tagstack ();

void tagstack_push (struct tagstack* tags, char* tag);
bool tagstack_pop (struct tagstack* tags, char* tag);

bool tagstack_is_empty (struct tagstack* tags);

void free_tagstack (struct tagstack* tags);


#endif
