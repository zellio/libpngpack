

#ifndef __LIBPNGPACK_IO_PNG_H__
#define __LIBPNGPACK_IO_PNG_H__ 1


#include "type/byte.h"
#include "type/png.h"

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


char *png_to_s(png_t *block);
int png_io_write(png_t *block, FILE *fp);
int png_io_read(png_t *block, FILE *fp);


#ifdef __cplusplus
};
#endif

#endif
