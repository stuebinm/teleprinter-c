#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct command* emph;
char* emph_method (struct charv** argv) {
	char* ret = malloc (argv[0]->length+9);
	sprintf (ret, "<em>%s</em>", argv[0]->array);
	return ret;
}

struct command* enquote;
char* enquote_method (struct charv** argv) {
	char* ret = malloc (argv[0]->length+6);
	sprintf (ret, "“%s”", argv[0]->array);
	return ret;
}

struct command* chapter;
char* chapter_method (struct charv** argv) {
	char* ret = malloc (argv[0]->length+9);
	sprintf (ret, "<h2>%s</h2>", argv[0]->array);
	return ret;
}

struct command* chbegin;
char* chbegin_method (struct charv** argv) {
	char* ret = malloc (argv[0]->length+16);
	printf ("%s\n", argv[1]->array);
	sprintf (ret, "<h4>%s</h4><b>%s</b>", argv[0]->array, argv[1]->array);
	return ret;
}


struct command* get_command (char* name) {
	
	if (!strcmp("emph", name) || !strcmp("thought", name)) {
		return emph;
	}
	if (!strcmp ("enquote", name)) return enquote;
	if (!strcmp ("chapter*", name)) return chapter;
	if (!strcmp ("chbegin", name)) return chbegin;
	return 0;
}


void commands_init () {
	emph = malloc (sizeof (struct command));
	emph->method = &emph_method;
	emph->argc = 1;
	
	enquote = malloc (sizeof (struct command));
	enquote->method = &enquote_method;
	enquote->argc = 1;
	
	chapter = malloc (sizeof (struct command));
	chapter->method = &chapter_method;
	chapter->argc = 1;
	
	chbegin = malloc (sizeof (struct command));
	chbegin->method = &chbegin_method;
	chbegin->argc = 2;

}
