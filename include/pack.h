

#ifndef __LIBPNGPACK_PACK_H__
#define __LIBPNGPACK_PACK_H__ 1


#include "type/memblk.h"


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


typedef struct png_block png_block_t;
struct png_block {
    uint32_t length;
    uint32_t type;
    byte* data;
};


png_block_t* png_block_create(uint32_t type, uint32_t size, byte* data);
int png_block_destroy(png_block_t* block);


#ifdef __cplusplus
};
#endif

#endif
