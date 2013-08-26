

#include "io/png.h"
#include "io/png/block.h"

#include <stdlib.h>
#include <string.h>

char *png_to_s(png_t *png) {
    if (png == NULL)
        return NULL;

    size_t size = png->size;

    char *str = calloc(size + 1, sizeof(char));
    char *ptr = str;

    png_node_t *current_node = png->head;
    png_block_t *cpb = current_node->block;


    memcpy(ptr, cpb->type, 4);       ptr += 4;
    memcpy(ptr, cpb->crc,  4);       ptr += 4;

    size_t cpb_len;
    while ((current_node = current_node->next)) {
        cpb = current_node->block;
        cpb_len = png_block_get_length(cpb);

        memcpy(ptr, cpb->length, 4);       ptr += 4;
        memcpy(ptr, cpb->type,   4);       ptr += 4;
        memcpy(ptr, cpb->data,   cpb_len); ptr += cpb_len;
        memcpy(ptr, cpb->crc,    4);       ptr += 4;
    }


    return str;
}

int png_io_write(png_t *png, FILE *fp) {
    if (png == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    char *str = png_to_s(png);
    if (str == NULL)
        return -3;

    fwrite(str, sizeof(byte), png->size, fp);
    free(str);

    return 0;
}

int png_io_read(png_t *png, FILE *fp) {
    if (png == NULL)
        return -1;

    if (fp == NULL)
        return -2;

    rewind(fp);
    fseek(fp, 8, SEEK_CUR);      /* skip the header */

    png_block_t *block;
    while (!feof(fp)) {
        block = png_block_create_empty();
        png_block_io_read(block, fp);
        if (png_block_get_type(block) == PNG_BLOCK_CRITICAL_TYPE_IEND) {
            png_block_destroy(block);
            return 0;
        }
        png_add(png, block);
    }

    return -4;
}
