

CC =            gcc
CFLAGS =        -std=c11 -Wextra -Wall -pedantic -ggdb
LDFLAGS =       -lncurses -lpthread
INCDIR =        ../../chemu/include
OBJFILES =		main.o
LIB =           ../../chemu/bin/libchemu.a


all:	chemu-cli

chemu-cli:	$(OBJFILES) $(LIB)
	$(CC) -o $@ $+ $(LDFLAGS)


#.c.o:
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm $(OBJFILES)

realclean: clean
	rm chemu-cli
