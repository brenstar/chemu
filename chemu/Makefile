# paths

BINDIR=			bin
OBJDIR=			obj
SRCDIR=			src
INCDIR=			include

vpath %.c $(SRCDIR)

# Variables

ifeq ($(OS),Windows_NT)
	DLLEXT=dll
else
	DLLEXT=so
endif

CC =			gcc
CFLAGS_BASE =	-std=c11 -Wextra -Wall -pedantic -fPIC
CFLAGS =		$(CFLAGS_BASE)
OBJFILES =		$(addprefix $(OBJDIR)/,emulation.o instructions.o stack.o input.o memory.o)
LDFLAGS=		-shared

chemu: CFLAGS = $(CFLAGS_BASE) -D NDEBUG
chemu-debug: CFLAGS = $(CFLAGS_BASE)

# targets

all: chemu

chemu: $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@.$(DLLEXT) $+

chemu-debug: clean chemu

#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@


# utils

clean:
	rm -f $(OBJDIR)/*.o

realclean:        clean
	rm -f $(BINDIR)/chemu.$(DLLEXT)
