

#include "io/png.h"


char* png_to_s(png_t* png) {
    if (png == NULL)
        return NULL;

    size_t size = png->size;

    char *str = calloc(size + 1, sizeof(char));
    char *ptr = str;

    png_node_t *current_node = png->head;
    png_block_t *cpb = current_node->block;

    snprintf(ptr, 5, "%s", cpb->type);                   ptr += 4;
    snprintf(ptr, 5, "%s", cpb->crc);                    ptr += 4;

    size_t pb_length;
    while ((current_node = current_node->next)) {
        cpb = current_node->block;
        pb_length = png_block_get_length(cpb);

        snprintf(ptr, 5, "%s", cpb->length);             ptr += 4;
        snprintf(ptr, 5, "%s", cpb->type);               ptr += 4;
        snprintf(ptr, pb_length + 1, "%s", cpb->data);   ptr += pb_length;
        snprintf(ptr, 5, "%s", cpb->crc);                ptr += 4;
    }

    return str;
}

int png_io_write(png_t* png, FILE* fp) {
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

//int png_io_read(png_t* png, FILE* fp) {
//}
