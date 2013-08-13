CC=clang
RM=rm -Rf

CFLAGS=-ggdb -x c -std=c99 -c -Wall -Wextra -Werror -pedantic-errors -ferror-limit=1

SRCROOT=src
INCROOT=include
TSTROOT=test

OBJROOT=build

VPATH=$(SRCROOT):$(INCROOT):$(TSTROOT):$(OBJROOT)

.PHONY: all clean

all: typeblob ioblob

typeblob: memblk.type.o png_block.type.o png_node.type.o png.type.o \
	pngpack_context.type.o

ioblob: memblk.io.o png_block.io.o png.io.o

clean:
	$(RM) $(OBJROOT)/*.o


# Types
memblk.type.o: type/memblk.c type/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png_block.type.o: type/png/block.c type/png/block.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png_node.type.o: type/png/node.c type/png/node.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png.type.o: type/png.c type/png.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

pngpack_context.type.o: type/pngpack_context.c type/pngpack_context.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

# IO
memblk.io.o: io/memblk.c io/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png_block.io.o: io/png/block.c io/png/block.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

png.io.o: io/png.c io/png.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

# Other
io.o: io.c io.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)
