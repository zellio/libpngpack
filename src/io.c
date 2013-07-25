

#include "io.h"

#include <stdlib.h>
#include <stdio.h>


char* memblk_to_s(memblk_t* block) {
    if (block == NULL)
        return NULL;

    char* str = calloc(block->size + 1, sizeof(byte));
    if (str == NULL)
        return NULL;

    snprintf(str, block->size + 1, "%s", block->data);

    return str;
}

char* png_block_to_s(png_block_t* block) {
    if (block == NULL)
        return NULL;

    size_t block_size = png_block_length_as_i(block);
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

int memblk_fwrite(memblk_t* block, FILE* fp) {
    if (block == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    char* str = memblk_to_s(block);
    if (str == NULL)
        return -3;

    fwrite(str, sizeof(byte), block->size, fp);
    free(str);

    return 0;
}

int png_block_fwrite(png_block_t* block, FILE* fp) {
    if (block == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    char* str = png_block_to_s(block);
    if (str == NULL)
        return -3;

    fwrite(str, sizeof(byte), png_block_length_as_i(block) + 12, fp);
    free(str);

    return 0;
}
