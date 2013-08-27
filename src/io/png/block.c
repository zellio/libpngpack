
#include "io/png/block.h"

#include <stdlib.h>
#include <string.h>


char* png_block_to_s(png_block_t* block) {
    if (block == NULL)
        return NULL;

    size_t block_len = png_block_get_length(block);
    char* str = calloc(block_len + 13, sizeof(byte));
    if (str == NULL)
        return NULL;

    char* ptr = str;
    memcpy(ptr, block->length, 4);         ptr += 4;
    memcpy(ptr, block->type,   4);         ptr += 4;
    memcpy(ptr, block->data,   block_len); ptr += block_len;
    memcpy(ptr, block->crc,    4);

    return str;
}

char *png_block_inspect(png_block_t *block) {
    char *str = calloc(64, sizeof(char));
    char *ptr = str;

    ptr += snprintf(ptr, 22, "#<PNG::BLOCK: lenght=");
    ptr += snprintf(ptr, 17, "%d type=", png_block_get_length(block));
    ptr += snprintf(ptr, 8, "\"%s\" ", block->type);
    snprintf(ptr, 16, "crc=%#08x>", png_block_get_crc(block));

    return str;
}

int png_block_io_read(png_block_t *block, FILE *fp) {
    if (block == NULL)
        return -1;
    if (fp == NULL)
        return -2;

    byte *length_buffer = calloc(4, sizeof(byte));
    if (length_buffer == NULL)
        return -3;
    fread(length_buffer, sizeof(byte), 4, fp);

    byte *old_length = block->length;
    block->length = length_buffer;
    size_t length = png_block_get_length(block);

    byte *type_buffer = NULL;
    byte *data_buffer = NULL;
    byte *crc_buffer = NULL;

    type_buffer = calloc(4, sizeof(byte));
    if (type_buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;

    data_buffer = calloc(length, sizeof(byte));
    if (data_buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;

    crc_buffer = calloc(4, sizeof(byte));
    if (crc_buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;

    // Length already read in
    if (old_length != NULL)
        free(old_length);
    // Length already asinged

    fread(type_buffer, sizeof(byte), 4, fp);
    if (block->type != NULL)
        free(block->type);
    block->type = type_buffer;

    fread(data_buffer, sizeof(byte), length, fp);
    if (block->data != NULL)
        free(block->data);
    block->data = data_buffer;

    fread(crc_buffer, sizeof(byte), 4, fp);
    if (block->crc != NULL)
        free(block->crc);
    block->crc = crc_buffer;

    return 0;

 PNG_BLOCK_IO_READ_CLEANUP:
    free(block->length);
    block->length = old_length;

    if (type_buffer != NULL)
        free(type_buffer);

    if (data_buffer != NULL)
        free(data_buffer);

    if (crc_buffer != NULL)
        free(crc_buffer);

    return -4;
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
