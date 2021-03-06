#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"



// read a the whole line, stop if encouter eof
static char *readline(FILE *in, sip_error *error) 
{
	size_t size = DEFAULT_LINE_SIZE;
	char *str;

	if((str = malloc(size)) == NULL) {
		*error = ERROR_MALLOC; 
		return NULL;
	}

	char *pos;
	char *temp_alloc;
	char lread = 0;

	while(fgets(str + size - DEFAULT_LINE_SIZE, DEFAULT_LINE_SIZE, in)) {
		lread = 1;
		if((pos = strchr(str + size - DEFAULT_LINE_SIZE, '\n')) != NULL) { 
			*pos = '\0';
			break;
		}
		
		size = size + DEFAULT_LINE_SIZE - 1;
		temp_alloc = realloc(str,size); 

		if(temp_alloc == NULL) {
			*error = ERROR_MALLOC;
			free(str);
			return NULL;
		}
	}
	
	if(!lread) {
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

void sip_parse_file(FILE *file, void (*handler)(enum line_type type, 
	char *key_section, char *value), sip_error *error)
{

	char *section;
	char *temp_sec;

	char *line;

	char *pos;
	char *key, *value;

	size_t ssize = DEFAULT_SECTION_SIZE;
	size_t size;

	if((section = malloc(ssize)) == NULL) {
		*error = ERROR_MALLOC;
		return;
	}

	while((line = readline(file, error)) != NULL) {
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
		else if((pos = strchr(line, '=')) != NULL) {
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
