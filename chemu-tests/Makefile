
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

repltest: $(addprefix $(OBJDIR)/,repl_test.o emulation.o instructions.o memory.o input.o stack.o)
	$(CC) -o $(BINDIR)/$@ $+

displaytest: $(addprefix $(OBJDIR)/,chip_display_test.o display.o)
	$(CC) -o $(BINDIR)/$@ $+

displaytest-curses: $(addprefix $(OBJDIR)/,displaytest_curses.o display.o)
	$(CC) -o $(BINDIR)/$@ $+ -lncurses

decodetest: $(addprefix $(OBJDIR)/,decode.o decodetest.o instructions.o)
	$(CC) -o $(BINDIR)/$@ $+

#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(UNIT_INC) -c $< -o $@

# utils:

clean:
	rm -f $(OBJDIR)/*.o

realclean: clean
	rm -f $(BINDIR)/memtest*
	rm -f $(BINDIR)/stacktest*
	rm -f $(BINDIR)/repltest*
	rm -f $(BINDIR)/displaytest*
