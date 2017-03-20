#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>


int column = 0;

void error_exit (int code, char* msg) {
	if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
	fprintf (ERRFILE, " >>> ERROR: %s\n", msg);
	exit (code);
}

void command_error (char* msgs, char* command, char* msge) {
	if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
	fprintf (ERRFILE, " >>> ERROR: %s", msgs);
	fprintf (ERRFILE, "\"\\%s\"", command);
	fprintf (ERRFILE, "%s\n\texiting …\n", msge);
	exit (UNKNOWN_COMMAND_ERROR);
}

void newcommand_error (char* msg) {
    if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
    fprintf (ERRFILE, " >>> ERROR: %s\n\t exiting …\n", msg);
    exit (NEWCOMMAND_ERROR);
}

void leaving_env_error (char* name) {
	if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
	fprintf (ERRFILE, " >>> ERROR: Premature end of environment \"%s\"!\n\texiting …\n", name);
	exit (PREMATURE_ENV_ERROR);
}

void eof_error () {
	if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
	fprintf (ERRFILE, " >>> ERROR: Premature end of file.\n\texiting …\n");
	exit (EOF_ERROR);
}

void parse_error () {
	if (column != 0) {
        fprintf (ERRFILE, "\n");
    }
    fprintf (ERRFILE, " >>> ERROR: invalid syntax!\n\texiting …\n");
	exit (PARSE_ERROR);
}

void msg_log (char* name, char* msg) {
    int l = strlen (name) + strlen (msg) + 4;
    column += l;
    if (column >= LOGWIDTH) {
        fprintf (LOGFILE, "\n");
        column = l;
    }
    fprintf (LOGFILE, "[%s: %s]", name, msg);
}

void msg_logc (char* name, char c) {
    int l = strlen (name) + 5;
    column += l;
    if (column >= LOGWIDTH) {
        fprintf (LOGFILE, "\n");
        column = l;
    }
    fprintf (LOGFILE, "[%s: %c]", name, c);
}

void macro_log (char* name) {
    int l = strlen (name) + 10;
    column += l;
    if (column >= LOGWIDTH) {
        fprintf (LOGFILE, "\n");
        column = l;
    }
    fprintf (LOGFILE, "[macro: \\%s]", name);
}

void msg_simple (char* str) {
    fprintf (LOGFILE, "%s", str);
}

inline void non_doc_error () {
    error_exit (NON_DOC_ERROR, "no text allowed outside of doc.");
}
