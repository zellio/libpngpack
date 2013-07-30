

#ifndef __LIBPNGPACK_TYPE_PNG_BLOCK_H__
#define __LIBPNGPACK_TYPE_PNG_BLOCK_H__ 1


#include "type/byte.h"
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


typedef struct png_block png_block_t;
struct png_block {
    byte* length;
    byte* type;
    byte* data;
    byte* crc;
};


png_block_t* png_block_create_empty(void);
png_block_t* png_block_create(uint32_t type, uint32_t size, byte* data);
int png_block_destroy(png_block_t* block);

int32_t png_block_length_as_i(png_block_t* block);
int32_t png_block_type_as_i(png_block_t* block);
int32_t png_block_crc_as_i(png_block_t* block);



#ifdef __cplusplus
};
#endif

#endif
