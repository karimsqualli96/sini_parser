#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"



// read a the whole line, stop if encouter eof
static char *readline(FILE *in, sid_error *error) 
{
	#define MAX_SIZE	64	
	size_t size = MAX_SIZE;
	char *str;

	if((str = malloc(sizeof *str * size)) == NULL) {
		*error = ERROR_MALLOC; 
		return NULL;
	}

	char *pos;
	char read = 0;

	while(fgets(str + size - MAX_SIZE, MAX_SIZE, in)) {
		read = 1;
		if(pos = strchr(str, '\n')) { 
			*pos = '\0';
			break;
		}
		
		size = size + MAX_SIZE - 1;
		str = realloc(str, sizeof *str * size); 
	}
	
	if(!read) {
		free(str);
		return NULL;
	}

	return str;
}

// strip line for extra spaces and comments
static void line_strip(char *str)
{
	char *iterstr = str;
	char *sstr, *estr;
	
	while(*iterstr && isspace(*iterstr))
		++iterstr;

	sstr = iterstr;

	while(*iterstr) {
		if(!isspace(*iterstr)) {
			estr = iterstr;
			if(*estr == '#' || *estr == ';') {
				--estr;
				break;
			}
		}
		++iterstr;
	}
	
	*(estr+1) = '\0';
	memmove(str, sstr, estr - sstr + 2);
	estr = str + (estr - sstr);

	while(estr > str && isspace(*estr))
		--estr;
	
	*(estr+1) = '\0';
}

void parse_file(FILE *file, void (*handler)(enum line_type type, 
	char *key_section, char *value), sid_error *error)
{

	char *section;
	char *temp_sec;

	char *line;

	char *pos;
	char *key, *value;

	size_t ssize = DEFAULT_SECTION_SIZE;
	size_t size;

	if((section = malloc(sizeof *section * ssize)) == NULL) {
		*error = ERROR_MALLOC;
		return;
	}

	while(line = readline(file, error)) {
		if(*error)
			goto FREE;

		// the line is a new section
		line_strip(line);
		if(line[0] == '[') {
			size = strlen(line);

			if(size > ssize) {
				if((temp_sec = realloc(section, 
					ssize += ssize - size)) == NULL)
				{
					*error =  ERROR_MALLOC;
					goto FREE;
				}

				section = temp_sec;
			}

			if(line[size - 1] == ']') {
				line[size -1] = '\0'; 
				strcpy(section, line + 1);
				handler(e_section, section, NULL);
			}
			else *error = ERROR_PARSE;
		}
		// key_value pair
		else if(pos = strchr(line, '=')) {
			*(pos - 1) = '\0';
			key = line;
			value = pos + 1;

			line_strip(key);
			line_strip(value);

			handler(e_key_value, key, 
				value);
		}
		else if(line[0] != '\0')
			*error = ERROR_PARSE;
			
		FREE:
		free(line);

		
		if(*error)
			break;
	}

	free(section);
}
