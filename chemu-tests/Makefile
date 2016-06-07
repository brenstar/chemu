
# paths

BINDIR=		bin
OBJDIR=		obj
SRCDIR=		src

UNIT_SRC=	../chemu/src
UNIT_INC=	../chemu/include

vpath %.c $(SRCDIR):$(UNIT_SRC)

# Variables

CC = 		gcc
CFLAGS =	-std=c11 -Wextra -Wall -pedantic


# targets

memtest: $(addprefix $(OBJDIR)/,memory.o chip_mem_test.o)
	$(CC) -o $(BINDIR)/$@ $+

stacktest: $(addprefix $(OBJDIR)/,stack.o chip_stack_test.o)
	$(CC) -o $(BINDIR)/$@ $+

repltest: $(addprefix $(OBJDIR)/,repl_test.o emulation.o instructions.o memory.o stack.o)
	$(CC) -o $(BINDIR)/$@ $+


#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(UNIT_INC) -c $< -o $@
