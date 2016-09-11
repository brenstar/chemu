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
OBJFN =			emulation.o decode.o display.o instructions_a.o input.o instructions_i.o instructions_r.o instructions_v.o memory.o logger.o stack.o timer.o
OBJFILES =		$(addprefix $(OBJDIR)/,$(OBJFN))
LDFLAGS=		-shared
DEFINES=		$(addprefix -D,NDEBUG _INLINE_)

chemu: CFLAGS = $(CFLAGS_BASE)
chemu-debug: CFLAGS = $(CFLAGS_BASE)

# targets

all: chemu

chemu: $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@.$(DLLEXT) $(DEFINES) $+

chemu-debug: clean chemu

chemu-static: $(OBJFILES)
	ar -cvr $(BINDIR)/libchemu.a $(OBJDIR)/*.o

#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) $(DEFINES) -c $< -o $@


# utils

clean:
	rm -f $(OBJDIR)/*.o

realclean:        clean
	rm -f $(BINDIR)/chemu.$(DLLEXT)
