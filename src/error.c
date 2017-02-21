#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void error_exit (int code, char* msg) {
	printf ("%s\n", msg);
	exit (code);
}

void unknown_command_exit (char* command) {
	printf ("Unknown command \"\\%s\"!\n\texiting …\n", command);
	exit (1);
}
