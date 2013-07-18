
CC=clang
RM=rm -Rf

CFLAGS=-ggdb -x c -std=c99 -c -Wall -Wextra -Werror -pedantic-errors

SRCROOT=lib/src
INCROOT=lib/include
TSTROOT=lib/test

OBJROOT=build

VPATH=$(SRCROOT):$(INCROOT):$(TSTROOT):$(OBJROOT)

.PHONY: all clean

all: memblk.o

clean:
	$(RM) $(OBJROOT)/*.o
	$(RM) pngpack


memblk.o: type/memblk.c type/memblk.h
	$(CC) $(CFLAGS) -I$(INCROOT) -o $(OBJROOT)/$(@) $(<)
