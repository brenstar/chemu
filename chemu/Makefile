
#==============================================================================
# paths

PLATFORM := $(shell uname -m)-$(shell uname -s)

BINROOT:=       bin
BINDIR:=        $(BINROOT)/$(PLATFORM)
OBJROOT:=       obj
SRCDIR:=        src
INCDIR:=        include



vpath %.c $(SRCDIR)

#==============================================================================
# Variables

NAME := libchemu
DNAME := $(NAME).so
SNAME := $(NAME).a

DTARGET := $(BINDIR)/$(DNAME)
STARGET := $(BINDIR)/$(SNAME)

DOBJDIR := $(OBJROOT)/$(DNAME)-$(PLATFORM)
SOBJDIR := $(OBJROOT)/$(SNAME)-$(PLATFORM)

CFLAGS :=       -std=c11 -Wextra -Wall -pedantic
ARFLAGS := 		-cvr

SRCFILES := $(wildcard $(SRCDIR)/*.c)
SRCFILES := $(filter-out $(SRCDIR)/instructions.c, $(SRCFILES))

DOBJFILES := $(SRCFILES:$(SRCDIR)/%.c=$(DOBJDIR)/%.o)
SOBJFILES := $(SRCFILES:$(SRCDIR)/%.c=$(SOBJDIR)/%.o)

LDFLAGS=

INCLUDES=       -I$(INCDIR)

MKDIR_P = mkdir -p

#==============================================================================
# Definitions

DEFINES := -D_GNU_SOURCE

# No debug, disables debug output
DEFINES += -DNDEBUG

# enables inline functions
DEFINES += -D_INLINE_

#==============================================================================
# Targets

.PHONY=all chemu chemu-static clean realclean

all: chemu chemu-static

chemu: CFLAGS += -fPIC
chemu: LDFLAGS += -shared
chemu: $(DTARGET)

chemu-static: $(STARGET)

$(DTARGET): $(DOBJFILES) | $(BINDIR)
	$(CC) $(LDFLAGS) $(DOBJFILES) -o $@

$(STARGET): $(SOBJFILES) | $(BINDIR)
	$(AR) $(ARFLAGS) $@ $(SOBJFILES)


COMPILE = $(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(DOBJDIR)/%.o: %.c | $(DOBJDIR)
	$(COMPILE)

$(SOBJDIR)/%.o: %.c | $(SOBJDIR)
	$(COMPILE)

#.c.o:
#$(DOBJDIR)/%.o: %.c
#	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(DOBJDIR) $(SOBJDIR) $(BINDIR):
	$(MKDIR_P) $@

# $(SOBJDIR):
# 	$(MKDIR_P) $@
#
# $(BINDIR):
# 	$(MKDIR_P) $@

#==============================================================================
# utils

clean:
	$(RM) -f $(DOBJDIR)/*.o $(SOBJDIR)/*.o

realclean: clean
	$(RM) -f $(STARGET) $(DTARGET)
