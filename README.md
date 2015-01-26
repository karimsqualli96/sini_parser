Simple ini parser
========

as it name implie is a simple library to make reading ini files as simple as calling a function.
sip provide a very simple api, that consist of calling the function `sip_parse_file` to parser your ini file.

Code Example
------------

this is the declaration of the main function that we will use to parse the ini file :

```c
void sip_parse_file(FILE *file, void (*handler)(enum line_type type, 
	char *key_section, char *value), sip_error *error)
```

this is a simple exemple of how to use this library :

```c
#include "parser.h"

// this is our handler that intercept each section|key-value when its read from the ini file
// we just print what we read from the ini file to the standard output.
void handler(enum line_type type, char *key_section, char *value)
{
  // type indicate that we have a section name 
	if(type == e_section) 
		printf("section : %s", key_section);
	// type indicate that we have key-value
	else 
		printf("key = %s\nvalue = %s\n", key_section, value);
	
	putchar('\n');
}

int main(int argc, char **argv)
{
  // we use a variable error to check wheter an error happened while reading the ini file
	sid_error error = 0;
	sip_parse_file(stdin, handler, &error);

	if(error) 
		printf("error ocured while reading ini file");

	return error;
}
```

Installation
------------

go to the project directory and run `make`.
copy the `libsip.a` and `parser.h` from the directory `out` to where you want to use the library.

suppose you have your source file that contain the main function `main.c` and `libsip.a`and `parser.h` in the same directory,
to compile you project you with gcc:

``` gcc main.c -L. -lsip ```

Contribute
----------

- Issue Tracker: https://github.com/karimsqualli96/sini_parser/issues
- Source Code: https://github.com/karimsqualli96/sini_parser

License
-------
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004
