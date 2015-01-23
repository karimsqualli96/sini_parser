SHELL = /bin/sh

CC := gcc
CFLAGS := -Wall


libsip.a : obj/parser.o
	ar cr $@ obj/parser.o

obj/parser.o : src/parser.c src/parser.h
	$(CC) $(CFLAGS) $< -c -o obj/parser.o

clean :
	rm -f obj/* libsip.a
