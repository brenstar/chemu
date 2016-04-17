# paths

BINDIR=			bin
SRCDIR=			src
INCDIR=			include

vpath %.c $(SRCDIR)
#vpath %.h $(SRCDIR)

# Variables

CC =			gcc
CFLAGS =		-std=c99 -Wextra -Wall -pedantic
OBJFILES =		$(addprefix $(BINDIR)/,chemu.o chipemu.o instructions.o chipstack.o)
LDFLAGS=

# targets

all: chemu insttest

chemu: $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@ $+

insttest: $(addprefix $(BINDIR)/,insttest.o instructions.o chipemu.o chipstack.o)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@ $+


#.c.o:
$(BINDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@


# utils

clean:
	cd $(BINDIR) && \
	-/bin/rm -f *.o

realclean:        clean
	cd $(BINDIR) && \
	-/bin/rm -f chemu display
