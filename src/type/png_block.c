

#include "type/png_block.h"

#include <stdlib.h>
#include <zlib.h>


static inline void unpack_uint32(byte* ptr, uint32_t* ui32) {
    *ui32 = 0;
    *ui32 += ((*ptr++) << 24);
    *ui32 += ((*ptr++) << 16);
    *ui32 += ((*ptr++) << 8);
    *ui32 += ((*ptr++) << 0);
}

static inline void pack_uint32(byte* ptr, uint32_t ui32) {
    *ptr++ = (ui32 >> 24) & 0xff;
    *ptr++ = (ui32 >> 16) & 0xff;
    *ptr++ = (ui32 >> 8) & 0xff;
    *ptr++ = (ui32) & 0xff;
}

png_block_t* png_block_create_empty(void) {
    png_block_t* block = calloc(1, sizeof(png_block_t));
    if (block == NULL)
        goto PNG_BLOCK_CREATE_EMPTY_CLEANUP;

    block->length = calloc(4, sizeof(byte));
    if (block->length == NULL)
        goto PNG_BLOCK_CREATE_EMPTY_CLEANUP;

    block->type = calloc(4, sizeof(byte));
    if (block->type == NULL)
        goto PNG_BLOCK_CREATE_EMPTY_CLEANUP;

    block->data = NULL;

    block->crc = calloc(4, sizeof(byte));
    if (block->crc == NULL)
        goto PNG_BLOCK_CREATE_EMPTY_CLEANUP;

    return block;

 PNG_BLOCK_CREATE_EMPTY_CLEANUP:
    png_block_destroy(block);
    return NULL;
}

png_block_t* png_block_create(uint32_t type, uint32_t size, byte* data) {
    png_block_t* block = png_block_create_empty();
    if (block == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;

    png_block_set_length(block, size);
    png_block_set_type(block, type);

    byte* data_ptr = calloc(size, sizeof(byte));
    if (data_ptr == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;
    block->data = data_ptr;
    for (; size; size--)
        *data_ptr++ = *data++;

    png_block_calculate_crc(block);

    return block;

 PNG_BLOCK_CREATE_CLEANUP:
    png_block_destroy(block);
    return NULL;
}

int png_block_destroy(png_block_t* block) {
    if (block == NULL)
        return -1;

    if (block->length != NULL) {
        free(block->length);
        return -2;
    }

    if (block->type != NULL) {
        free(block->type);
        return -3;
    }

    if (block->data != NULL) {
        free(block->data);
        return -4;
    }

    if (block->crc != NULL) {
        free(block->crc);
        return -5;
    }

    free(block);

    return 0;
}

int png_block_calculate_crc(png_block_t* block) {
    if (block == NULL)
        return -1;

    size_t length = png_block_get_length(block);
    uint64_t crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, block->type, 4);
    crc = crc32(crc, block->data, length);

    png_block_set_crc(block, crc);

    return 0;
}

uint32_t png_block_get_length(png_block_t* block) {
    uint32_t ui32;
    unpack_uint32(block->length, &ui32);
    return ui32;
}
uint32_t png_block_set_length(png_block_t* block, uint32_t value) {
    pack_uint32(block->type, value);
    return value;
}

uint32_t png_block_get_type(png_block_t* block) {
    uint32_t ui32;
    unpack_uint32(block->type, &ui32);
    return ui32;
}
uint32_t png_block_set_type(png_block_t* block, uint32_t value) {
    pack_uint32(block->type, value);
    return value;
}

uint32_t png_block_get_crc(png_block_t* block) {
    uint32_t ui32;
    unpack_uint32(block->crc, &ui32);
    return ui32;
}
uint32_t png_block_set_crc(png_block_t* block, uint32_t value) {
    pack_uint32(block->type, value);
    return value;
}
