
CC=clang
RM=rm -Rf

CFLAGS=-ggdb -x c -std=c99 -c -Wall -Wextra -Werror -pedantic-errors

SRCROOT=src
INCROOT=include
TSTROOT=test

OBJROOT=build

VPATH=$(SRCROOT):$(INCROOT):$(TSTROOT):$(OBJROOT)

.PHONY: all clean

all: memblk.o png_block.o io.o pack.o

clean:
	$(RM) $(OBJROOT)/*.o
	$(RM) pngpack


memblk.o: type/memblk.c type/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png_block.o: type/png_block.c type/png_block.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

io.o: io.c io.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)
