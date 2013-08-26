

#include "type/png/node.h"

#include <stdlib.h>


png_node_t *png_node_create(png_block_t *block) {
    png_node_t *node = calloc(1, sizeof(png_node_t));
    if (node == NULL)
        return NULL;

    node->block = block;

    return node;
}

int png_node_destroy(png_node_t *node) {
    if (node == NULL)
        return -1;
    free(node);
    return 0;
}

png_block_t *png_node_sentinel_head() {
    static png_block_t *head_block = NULL;
    if (head_block == NULL) {
        head_block = png_block_create_empty();
        png_block_set_type(head_block, 0x89504E47);
        png_block_set_crc(head_block, 0x0D0A1A0A);
    }
    return head_block;
}

png_block_t *png_node_sentinel_tail() {
    static png_block_t *tail_block = NULL;
    if (tail_block == NULL)
        tail_block = png_block_create(0x0, PNG_BLOCK_CRITICAL_TYPE_IEND, NULL);
    return tail_block;
}

png_node_t *png_node_create_head() {
    return png_node_create(png_node_sentinel_head());
}

png_node_t *png_node_create_tail() {
    return png_node_create(png_node_sentinel_tail());
}

int png_block_is_head(png_block_t *block) {
    png_block_t *head_block = png_node_sentinel_head();
    return png_block_compare(block, head_block) == 0;
}
int png_node_is_head(png_node_t *node) {
    return png_block_is_head(node->block);
}

int png_block_is_tail(png_block_t *block) {
    png_block_t *tail_block = png_node_sentinel_tail();
    return png_block_compare(block, tail_block) == 0;
}

int png_node_is_tail(png_node_t *node) {
    return png_block_is_tail(node->block);
}
