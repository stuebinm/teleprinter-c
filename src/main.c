#include <stdio.h>
#include <stdlib.h>

enum word_type_enum {
	text,
	comment,
	command,
	eof
};
typedef  enum word_type_enum word_type;
struct word_info_struct {
	 word_type type;
};

typedef struct word_info_struct word_info;


/**
 * fetch_word:
 * this will fetch the next word from the given @file. It also ignores comments.
 * @file: an input file.
 *
 * Returns: a char array with the word in it.
*/
char* fetch_word (FILE* file, word_info* info) {
	char c;
	 // skip leading whitspace
	while ( (c = (char) fgetc(file)) == ' ' || c == '\n' || c == '\t');
	
	 // check for file end
	if (c == EOF) {
		info->type = eof;
		return 0;
	}
	
	 // skip all comments
	if (c == '%') {
		while ( (c = skip_comment (c, file)) == '\n' || c == '%');
		 // skip any whitspace that comes after the comments, but before the word.
		if (c == ' ' || c == '\n' || c == 't')
		while ( (c = (char) fgetc(file)) == ' ' || c == '\n' || c == '\t');
	}
	
	int l = 1;
	char* ret = malloc (l);
	int i = 0;
	
	do {
		if (i == l) {
			l += 10;
			ret = realloc (ret, l);
		}
		ret [i] = c;
		i++;
	}
	while ( (c = (char) fgetc(file)) != ' ' && c != '\n' && c != '\t' && c != EOF);
	
	ret [i] = '\0';
	if (l>i+1) {
		ret = realloc (ret, i+1);
	}
	info->type = text;
	return ret;
}

/**
 * skip_comment:
 * Skips one (and exactly one) line of comment.
 *
*/
int skip_comment (int c, FILE* file) {
	while ( (c = (char) fgetc(file)) != '\n');
	c = fgetc (file);
	return c;
}

int main (int argc, char** argv) {
	
	if (argc != 2) {
		printf ("this program takes exactly one argument!\n");
		return 0;
	}
	
	FILE* input = fopen (argv[1], "r");
	
	if (input != 0) {
		printf ("opened file\n");
		
		char* word;
		
		int c = 0;
		word_info info;
		while ( (word = fetch_word (input, &info)) != 0) {
			if (info.type != comment) printf ("word number %d: \"%s\"\n", c, word);
			free (word);
			c++;
		}
		fclose (input);
	}
	else {
		printf ("Could not open file %s.\n", argv[1]);
	}
	return 0;
}

