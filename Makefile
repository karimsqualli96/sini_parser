SHELL = /bin/sh

CC := gcc
CFLAGS := -Wall

OBJSDIR := objs
OUTDIR := out

.PHONY : clean mkdirs

libsip.a : $(OBJSDIR)/parser.o
	ar cr -o $(OUTDIR)/$@ $(OBJSDIR)/parser.o 

$(OBJSDIR)/parser.o : src/parser.c src/parser.h mkdirs
	$(CC) $(CFLAGS) $< -c -o $(OBJSDIR)/parser.o

clean :
	rm -rf $(OBJSDIR) $(OUTDIR)

mkdirs :
	mkdir -p $(OBJSDIR) $(OUTDIR) 
