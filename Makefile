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

typeblob: type_memblk.o type_png_block.o type_pngpack_context.o \
	type_pngpack_context.o type_png.o

ioblob: io_memblk.o io_png_block.o

clean:
	$(RM) $(OBJROOT)/*.o


# Types
type_memblk.o: type/memblk.c type/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

type_png_block.o: type/png_block.c type/png_block.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

type_pngpack_context.o: type/pngpack_context.c type/pngpack_context.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

type_png.o: type/png.c type/png.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)


# IO
io_memblk.o: io/memblk.c io/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

io_png_block.o: io/png_block.c io/png_block.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)




# Other
io.o: io.c io.h#
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)
