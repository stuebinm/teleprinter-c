#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>


int column = 0;

void error_exit (int code, char* msg) {
	if (column != 0) {
        printf ("\n");
    }
	printf (" >>> ERROR: %s\n", msg);
	exit (code);
}

void command_error (char* msgs, char* command, char* msge) {
	if (column != 0) {
        printf ("\n");
    }
	printf (" >>> ERROR: %s", msgs);
	printf ("\"\\%s\"", command);
	printf ("%s\n\texiting …\n", msge);
	exit (UNKNOWN_COMMAND_ERROR);
}

void newcommand_error (char* msg) {
    if (column != 0) {
        printf ("\n");
    }
    printf (" >>> ERROR: %s\n\t exiting …\n", msg);
    exit (NEWCOMMAND_ERROR);
}

void leaving_env_error (char* name) {
	if (column != 0) {
        printf ("\n");
    }
	printf (" >>> ERROR: Premature end of environment \"%s\"!\n\texiting …\n", name);
	exit (PREMATURE_ENV_ERROR);
}

void eof_error () {
	if (column != 0) {
        printf ("\n");
    }
	printf (" >>> ERROR: Premature end of file.\n\texiting …\n");
	exit (EOF_ERROR);
}

void parse_error () {
	if (column != 0) {
        printf ("\n");
    }
    printf (" >>> ERROR: invalid syntax!\n\texiting …\n");
	exit (PARSE_ERROR);
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
    int l = strlen (name) + 5;
    struct winsize w;
    ioctl (0, TIOCGWINSZ, &w);
    column += l;
    if (column >= w.ws_col) {
        fprintf (stderr, "\n");
        column = l;
    }
    fprintf (stderr, "[%s: %c]", name, c);
}

inline void non_doc_error () {
    error_exit (NON_DOC_ERROR, "no text allowed outside of doc.");
}
