

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

    byte *buffer = calloc(4, sizeof(byte));
    if (buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(buffer, sizeof(byte), 4, fp);
    block->length = buffer;
    size_t length = png_block_get_length(block);

    buffer = calloc(4, sizeof(byte));
    if (buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(buffer, sizeof(byte), 4, fp);
    block->type = buffer;

    buffer = calloc(length, sizeof(byte));
    if (buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(buffer, sizeof(byte), length, fp);
    block->data = buffer;

    //fseek(fp, 4, SEEK_CUR);
    //png_block_calculate_crc(block);
    buffer = calloc(4, sizeof(byte));
    if (buffer == NULL)
        goto PNG_BLOCK_IO_READ_CLEANUP;
    fread(buffer, sizeof(byte), 4, fp);
    block->crc = buffer;

    return 0;

 PNG_BLOCK_IO_READ_CLEANUP:
    if (block->length)
        memset(block->length, 0x00, 4);
    if (block->type)
        memset(block->type, 0x00, 4);
    if (block->data)
        memset(block->data, 0x00, length);
    if (block->crc)
        memset(block->crc, 0x00, 4);
    return -3;
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
