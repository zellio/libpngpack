

#ifndef __LIBPNGPACK_IO_MEMBLK_H__
#define __LIBPNGPACK_IO_MEMBLK_H__ 1


#include "type/byte.h"
#include "type/memblk.h"

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


char* memblk_to_s(memblk_t* block);
int memblk_io_write(memblk_t* block, FILE* fp);
int memblk_io_read(memblk_t* block, FILE* fp);


#ifdef __cplusplus
};
#endif

#endif
