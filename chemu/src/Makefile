
CC =		gcc
CFLAGS =	-std=c99 -Wextra -Wall -pedantic
OBJFILES =	chemu.o chipemu.o instructions.o chipstack.o

# targets

all: chemu

chemu: $(OBJFILES)
	$(CC) $(CFLAGS) -o chemu $(OBJFILES)

display: display.o
	$(CC) $(CFLAGS) -o display display.o

insttest: insttest.o chipemu.o instructions.o
	$(CC) $(CFLAGS) -o insttest insttest.o chipemu.o instructions.o

chemu.o:
#chipmem.o: chipmem.h
chipemu.o: chipemu.h instructions.h
display.o: display.h
chipstack.o: chipstack.h
instructions.o: instructions.h chipemu.h
insttest.o:


# utils

clean:
	-/bin/rm -f $(OBJFILES) core

realclean:        clean
	-/bin/rm -f chemu display
