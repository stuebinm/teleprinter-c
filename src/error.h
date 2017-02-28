#ifndef ERROR_H
#define ERROR_H

void error_exit (int code, char* msg);

void unknown_command_exit (char* command);

void parse_error ();

void msg_log (char* name, char* msg);
void msg_logc (char* name, char c);

#endif
