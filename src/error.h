#ifndef ERROR_H
#define ERROR_H

#define UNKNOWN_FILE_ERROR 1
#define UNKNOWN_COMMAND_ERROR 2
#define PARSE_ERROR 3
#define EOF_ERROR 4
#define PREMATURE_ENV_ERROR 5
#define NON_DOC_ERROR 6
#define NEWCOMMAND_ERROR 7

void error_exit (int code, char* msg);

void command_error (char* msgs, char* command, char* msge);
void newcommand_error (char* msg);

void parse_error ();
void leaving_env_error (char* name);
void eof_error ();

void msg_log (char* name, char* msg);
void msg_logc (char* name, char c);

void non_doc_error ();

#endif
