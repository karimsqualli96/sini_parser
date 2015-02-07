SHELL = /bin/sh

CC := gcc
CFLAGS := -Wall

SRCSDIR := src
OBJSDIR := objs
OUTDIR := out

SRCS := $(wildcard $(SRCSDIR)/*.c) 
OBJS := $(SRCS:$(SRCSDIR)/%.c=$(OBJSDIR)/%.o)


$(OUTDIR)/libsip.a : $(OBJS)
	@mkdir -p $(@D)
	ar cr -o $@ $^
	cp -f src/parser.h $(OUTDIR)

$(OBJSDIR)/%.o : $(SRCSDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -c -o $@  
	$(CC) -MM $< > $(patsubst %.c, %.d, $<) && \
	sed -i \ '1s|^|$(@D)/|' $(patsubst %.c, %.d, $<) 
# use sed to add path to object dependencies

.PHONY : clean cleanall
clean :
	rm -rf $(OBJSDIR) $(OUTDIR)

cleanall : clean
	-rm -rf $(wildcard $(SRCSDIR)/*.d)

-include $(SRCS:%.c=%.d)


