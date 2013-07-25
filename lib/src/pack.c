

#include "pack.h"


png_block_t* png_block_create(uint32_t type, uint32_t size, byte* data) {
    png_block_t* png_blk = calloc(1, sizeof(png_block_t));
    if (png_blk == NULL)
        return NULL;

    png_blk->type = type;
    png_blk->size = size;
    png_blk->data = data;

    return png_blk;
}

int png_block_destroy(png_block_t* block) {
    if (block == NULL)
        return -1;
    free(block);
    return 0;
}
