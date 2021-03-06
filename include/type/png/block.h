

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


#define PNG_BLOCK_BASE_SIZE 12

#define PNG_BLOCK_CRITICAL_TYPE_IDHR 0x49444852
#define PNG_BLOCK_CRITICAL_TYPE_PLTE 0x504C5445
#define PNG_BLOCK_CRITICAL_TYPE_IDAT 0x49444154
#define PNG_BLOCK_CRITICAL_TYPE_IEND 0x49454E44


typedef struct png_block png_block_t;
struct png_block {
    byte *length;
    byte *type;
    byte *data;
    byte *crc;
};


png_block_t *png_block_create_empty(void);
png_block_t *png_block_create(uint32_t length, uint32_t type, byte *data);
int png_block_destroy(png_block_t *block);

int png_block_compare(png_block_t *self, png_block_t *other);

int png_block_calculate_crc(png_block_t *block);

uint32_t png_block_get_length(png_block_t *block);
uint32_t png_block_set_length(png_block_t *block, uint32_t value);
uint32_t png_block_get_type(png_block_t *block);
uint32_t png_block_set_type(png_block_t *block, uint32_t value);
uint32_t png_block_get_crc(png_block_t *block);
uint32_t png_block_set_crc(png_block_t *block, uint32_t value);


#ifdef __cplusplus
};
#endif

#endif
