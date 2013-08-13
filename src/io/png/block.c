

#include "io/png/block.h"

#include <stdlib.h>


char* png_block_to_s(png_block_t* block) {
    if (block == NULL)
        return NULL;

    size_t block_size = png_block_get_length(block);
    char* str = calloc(block_size + 13, sizeof(byte));
    if (str == NULL)
        return NULL;

    char* ptr = str;

    snprintf(ptr, 5, "%s", block->length);               ptr += 4;
    snprintf(ptr, 5, "%s", block->type);                 ptr += 4;
    snprintf(ptr, block_size + 1, "%s", block->data);    ptr += block_size;
    snprintf(ptr, 5, "%s", block->crc);

    return str;
}

int png_block_io_write(png_block_t* block, FILE* fp) {
    if (block == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    char* str = png_block_to_s(block);
    if (str == NULL)
        return -3;

    fwrite(str, sizeof(byte), png_block_get_length(block) + 12, fp);
    free(str);

    return 0;
}

int png_block_io_read(png_block_t* block, FILE* fp) {
    if (block == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    byte* int_buffer;

    int_buffer = calloc(4, sizeof(byte));
    if (int_buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(int_buffer, sizeof(byte), 4, fp);
    block->length = int_buffer;

    size_t length = png_block_get_length(block);

    int_buffer = calloc(4, sizeof(byte));
    if (int_buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(int_buffer, sizeof(byte), 4, fp);
    block->type = int_buffer;

    block->data = calloc(sizeof(byte), length);
    if (block->data == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(block->data, sizeof(byte), length, fp);

    png_block_calculate_crc(block);

    return 0;

 PNG_BLOCK_IO_READ_CLEANUP:
    return -1;
}
