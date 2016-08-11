
# paths

BINDIR=		bin
OBJDIR=		obj
SRCDIR=		src

UNIT_SRC=	../chemu/src
UNIT_INC=	../chemu/include

vpath %.c $(SRCDIR):$(UNIT_SRC)

# Variables

CC = 		gcc
CFLAGS =	-std=c11 -Wextra -Wall -pedantic -ggdb


# targets

memtest: $(addprefix $(OBJDIR)/,memory.o chip_mem_test.o)
	$(CC) -o $(BINDIR)/$@ $+

stacktest: $(addprefix $(OBJDIR)/,stack.o chip_stack_test.o)
	$(CC) -o $(BINDIR)/$@ $+

repltest: $(addprefix $(OBJDIR)/,repl_test.o emulation.o decode.o display.o instructions_a.o input.o instructions_i.o instructions_r.o instructions_v.o memory.o logger.o stack.o timer.o)
	$(CC) -o $(BINDIR)/$@ $+ -lpthread

displaytest: $(addprefix $(OBJDIR)/,chip_display_test.o display.o)
	$(CC) -o $(BINDIR)/$@ $+

displaytest-curses: $(addprefix $(OBJDIR)/,displaytest_curses.o display.o)
	$(CC) -o $(BINDIR)/$@ $+ -lncurses

decodetest: $(addprefix $(OBJDIR)/,decode.o decodetest.o instructions.o)
	$(CC) -o $(BINDIR)/$@ $+

timertest: $(addprefix $(OBJDIR)/,timer.o timertest.o)
	$(CC) -o $(BINDIR)/$@ $+ -lpthread

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
