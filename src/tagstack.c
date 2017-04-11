#include "tagstack.h"

#include <stdlib.h>
#include <string.h>

#include "error.h"


struct tagstack* new_tagstack () {
    struct tagstack* ret = malloc (sizeof (struct tagstack));
    
    ret->next = 0;
    ret->data = 0;
    
    return ret;
}

void tagstack_push (struct tagstack* tags, char* tag) {
    if (tags->data == 0) {
        tags->data = tag;
        return;
    }
    struct tagstack* next = malloc (sizeof (struct tagstack));
    next->data = tags->data;
    next->next = tags->next;
    tags->next = next;
    tags->data = tag;
}


bool tagstack_pop (struct tagstack* tags, char* tag) {
    if (tags->data == 0) {
        error_exit (1, "premature end of tag!");
    }
    if (strcmp (tag, tags->data) != 0) {
        printf ("\nerror: '%s' != '%s': %i\n", tag, tags->data, strcmp (tag, tags->data));
        return false;
    }
    if (tags->next == 0) {
        free (tags->data);
        tags->data = 0;
        tags->next = 0;
        return true;
    }
    struct tagstack* old = tags->next;
    free (tags->data);
    tags->data = tags->next->data;
    tags->next = tags->next->next;
    free (old);
    
    return true;
}


bool tagstack_is_empty (struct tagstack* tags) {
    return tags->data == 0 && tags->next == 0;
}

void free_tagstack (struct tagstack* tags) {
    if (!tagstack_is_empty (tags)) {
	    error_exit (1, "Not all tags were closed!");
	}
	free (tags);
}




