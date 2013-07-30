

#ifndef __LIBPNGPACK_IO_PNG_BLOCK_H__
#define __LIBPNGPACK_IO_PNG_BLOCK_H__ 1


#include "type/byte.h"
#include "type/png_block.h"

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


char* png_block_to_s(png_block_t* block);
int png_block_io_write(png_block_t* block, FILE* fp);
int png_block_io_read(png_block_t* block, FILE* fp);


#ifdef __cplusplus
};
#endif

#endif
