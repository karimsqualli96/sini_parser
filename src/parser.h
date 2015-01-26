typedef enum line_type {
	e_section,
	e_key_value,
}Type;


typedef unsigned int sip_error;

#define ERROR_MALLOC	1	
#define ERROR_PARSE	2	

#define DEFAULT_SECTION_SIZE	32
#define DEFAULT_LINE_SIZE	64

void parse_file(FILE *file, void (*handler)(enum line_type type,
	char *key_section, char *value), sip_error *error);
