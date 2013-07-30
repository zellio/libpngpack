

#include "type/png_block.h"

#include <stdlib.h>
#include <zlib.h>


png_block_t* png_block_create_empty(void) {
    png_block_t* png_block = calloc(1, sizeof(png_block_t));
    if (png_block == NULL)
        return NULL;
    return png_block;
}

png_block_t* png_block_create(uint32_t type, uint32_t length, byte* data) {
    png_block_t* png_block = png_block_create_empty();
    if (png_block == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;

    byte* length_ptr = calloc(4, sizeof(byte));
    if (length_ptr == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;
    png_block->length = length_ptr;
    *length_ptr++ = (length >> 24) & 0xff;
    *length_ptr++ = (length >> 16) & 0xff;
    *length_ptr++ = (length >> 8) & 0xff;
    *length_ptr++ = (length) & 0xff;

    byte* type_ptr = calloc(4, sizeof(byte));
    if (type_ptr == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;
    png_block->type = type_ptr;
    *type_ptr++ = (type >> 24) & 0xff;
    *type_ptr++ = (type >> 16) & 0xff;
    *type_ptr++ = (type >> 8) & 0xff;
    *type_ptr++ = (type) & 0xff;

    byte* data_ptr = calloc(length, sizeof(byte));
    if (data_ptr == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;
    png_block->data = data_ptr;
    for (size_t size = length; size; size--)
        *data_ptr++ = *data++;

    byte* crc_ptr = calloc(4, sizeof(byte));
    if (crc_ptr == NULL)
        goto PNG_BLOCK_CREATE_CLEANUP;
    uint64_t crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, png_block->type, 4);
    crc = crc32(crc, png_block->data, length);
    png_block->crc = crc_ptr;
    *crc_ptr++ = (crc >> 24) & 0xff;
    *crc_ptr++ = (crc >> 16) & 0xff;
    *crc_ptr++ = (crc >> 8) & 0xff;
    *crc_ptr++ = (crc) & 0xff;

    return png_block;

 PNG_BLOCK_CREATE_CLEANUP:
    png_block_destroy(png_block);
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

static inline int32_t bytes_to_int(byte* ptr) {
    int32_t ui32 = 0;
    ui32 += ((*ptr++) << 24);
    ui32 += ((*ptr++) << 16);
    ui32 += ((*ptr++) << 8);
    ui32 += ((*ptr++) << 0);
    return ui32;
}

int32_t png_block_length_as_i(png_block_t* block) {
    return bytes_to_int(block->length);
}

int32_t png_block_type_as_i(png_block_t* block) {
    return bytes_to_int(block->type);
}

int32_t png_block_crc_as_i(png_block_t* block) {
    return bytes_to_int(block->crc);
}
