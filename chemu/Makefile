# paths

BINDIR=			bin
OBJDIR=			obj
SRCDIR=			src
INCDIR=			include

vpath %.c $(SRCDIR)
#vpath %.h $(SRCDIR)

# Variables

CC =			gcc
CFLAGS =		-std=c11 -Wextra -Wall -pedantic -fPIC
OBJFILES =		$(addprefix $(OBJDIR)/,emulation.o instructions.o stack.o input.o memory.o)
LDFLAGS=		-shared

# targets

all: chemu

chemu: $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@.so $+

#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@


# utils

clean:
	cd $(BINDIR) && \
	/bin/rm -f *.o

realclean:        clean
	cd $(BINDIR) && \
	/bin/rm -f chemu display
