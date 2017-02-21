#ifndef COMMANDS_H
#define COMMANDS_H

#include "charv.h"


typedef char* (command_exec) (struct charv** argv);

struct command {
	int argc;
	command_exec* method;
};

struct command* get_command (char* name);

void commands_init ();


#endif
