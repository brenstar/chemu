
#==============================================================================
# paths

BINDIR=         bin
OBJDIR=         obj
SRCDIR=         src
INCDIR=         include

vpath %.c $(SRCDIR)

#==============================================================================
# Variables

CC =            gcc
CFLAGS =        -std=c11 -Wextra -Wall -pedantic -fPIC
OBJFN =         emulation.o decode.o display.o instructions_a.o input.o \
                instructions_i.o instructions_r.o instructions_v.o memory.o \
                logger.o stack.o timer.o
OBJFILES =      $(addprefix $(OBJDIR)/,$(OBJFN))
LDFLAGS=        -shared -lsthreads

LIBSTHREADSDIR= lib/sthreads
LIBSTHREADS=    $(LIBSTHREADSDIR)/bin/libsthreads.a

INCLUDES=       -I$(INCDIR) -I$(LIBSTHREADSDIR)/include

LIBS=           -L$(LIBSTHREADSDIR)/bin

#==============================================================================
# Definitions

DEFINES=

# No debug, disables debug output
DEFINES += -DNDEBUG

# enables inline functions
DEFINES += -D_INLINE_

#==============================================================================
# Targets

.PHONY=chemu chemu-static clean realclean

chemu: $(BINDIR)/chemu.so

chemu-static: $(BINDIR)/libchemu.a

$(BINDIR)/chemu.so: $(OBJFILES) $(LIBSTHREADS)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $(OBJFILES)

$(BINDIR)/libchemu.a: $(OBJFILES)
	ar -cvr $@ $+

$(LIBSTHREADS):
	cd $(LIBSTHREADSDIR) && $(MAKE)


#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@


#==============================================================================
# utils

clean:
	rm -f $(OBJDIR)/*.o

realclean:        clean
	rm -f $(BINDIR)/chemu.so
