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
OBJFILES =		$(addprefix $(OBJDIR)/,chemu.o chipemu.o instructions.o chipstack.o)
LDFLAGS=		-shared

# targets

all: chemu insttest

chemu: $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@.so $+

#insttest: $(addprefix $(BINDIR)/,insttest.o instructions.o chipemu.o chipstack.o)
#	$(CC) $(LDFLAGS) -o $(BINDIR)/$@ $+

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
