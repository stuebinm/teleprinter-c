#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define LINE_LENGTH 50

int column = 0;

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
    int l = strlen (name) + strlen (msg) + 4;
    struct winsize w;
    ioctl (0, TIOCGWINSZ, &w);
    column += l;
    if (column >= w.ws_col) {
        fprintf (stderr, "\n");
        column = l;
    }
    fprintf (stderr, "[%s: %s]", name, msg);
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
