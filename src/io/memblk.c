

#include "io/memblk.h"

#include <stdlib.h>


char* memblk_to_s(memblk_t* block) {
    if (block == NULL)
        return NULL;

    char* str = calloc(block->size + 1, sizeof(byte));
    if (str == NULL)
        return NULL;

    snprintf(str, block->size + 1, "%s", block->data);

    return str;
}

int memblk_io_write(memblk_t* block, FILE* fp) {
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

int memblk_io_read(memblk_t* block, FILE* fp) {
    if (block == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    size_t size = block->size;

    fread(block->data, sizeof(byte), size, fp);

    return memblk_rewind(block);
}
