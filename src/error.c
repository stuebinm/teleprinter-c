#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 50

char column = 0;

void error_exit (int code, char* msg) {
	printf (" >>> ERROR: %s\n", msg);
	exit (code);
}

void unknown_command_exit (char* command) {
	if (column != 0) {
        printf ("\n");
    }
	printf (" >>> ERROR: Unknown command \"\\%s\"!\n\texiting …\n", command);
	exit (1);
}

void parse_error () {
	if (column != 0) {
        printf ("\n");
    }
    printf (" >>> ERROR: invalid syntax!\n\texiting …\n");
	exit (1);
}

void msg_log (char* name, char* msg) {
    fprintf (stderr, "[%s: %s]", name, msg);
    if (column > LINE_LENGTH) {
        printf ("\n");
        column = 0;
    }
    else {
        column += strlen (name)+strlen(msg)+10;
    }
}

void msg_logc (char* name, char c) {
    /*printf ("hello: %s\n", last);
    if (!strcmp (name, last)) {
        counter += 1;
        if (counter == 2) printf (" × 2\n");
        else printf ("\b\b%d\n", counter);
    } else {
        counter = 1;
//        last = malloc (strlen (name) + 1);
//        strcpy (last, name);
        printf ("[%s: %c]\n", name, c);
    }*/
}
