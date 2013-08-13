

#include "type/png.h"

#include <stdlib.h>


png_t *png_create(void) {
    png_t* png = calloc(1, sizeof(png));
    if (png == NULL)
        return 0;

    png_node_t *head = png_node_create_head();
    png_node_t *tail = png_node_create_tail();

    head->prev = NULL;
    head->next = tail;

    tail->prev = head;
    tail->next = NULL;

    png->head = head;
    png->tail = tail;

    png->length = 2;
    png->size = PNG_BASE_SIZE;

    return png;
}

int png_destroy(png_t *png) {
    if (png == NULL)
        return -1;

    png_clear(png);
    free(png);

    return 0;
}

int png_clear(png_t *png) {
    if (png == NULL)
        return -1;

    png_node_t *head = png->head;
    png_node_t *tail = png->tail;

    png_node_t *current_node = head->next;
    png_node_t *ptr = current_node;

    while (!(png_node_is_tail(current_node))) {
        ptr = current_node;
        current_node = current_node->next;
        png_node_destroy(ptr);
    }

    head->prev = NULL;
    head->next = tail;

    tail->prev = head;
    tail->next = NULL;

    png->length = 2;
    png->size = PNG_BASE_SIZE;

    return 0;
}

png_node_t *__png_get_at(png_t* png, size_t index) {
    if (png == NULL)
        return NULL;

    if (png->length <= index)
        return NULL;

    png_node_t *current_node = png->head;
    while (index-- && (current_node = current_node->next));

    if (current_node)
        return current_node;
    return NULL;
}

png_block_t *png_get_at(png_t* png, size_t index) {
    png_node_t *png_node = __png_get_at(png, index);
    if (png_node != NULL)
        return png_node->block;
    return NULL;
}

int png_index_of(png_t* png, png_block_t* block) {
    if (png == NULL || block == NULL)
        return -1;

    int index = 0;
    for (png_node_t* current = png->head; current; current = current->next) {
        if (png_block_compare(block, current->block) == 0)
            return index;
        index++;
    }

    return -1;
}

int png_contains(png_t *png, png_block_t *block) {
    return (png_index_of(png, block) != -1);
}

int png_add(png_t *png, png_block_t *block) {
    if (png == NULL)
        return -1;

    if (block == NULL)
        return -2;

    png_node_t *node = png_node_create(block);
    if (node == NULL)
        return -3;

    png_node_t *tail = png->tail;
    png_node_t *prev = tail->prev;

    prev->next = node;
    node->prev = prev;
    node->next = tail;
    tail->prev = node;

    png->length += 1;

    png->size += PNG_BLOCK_BASE_SIZE + png_block_get_length(block);

    return 0;
}

int png_add_at(png_t *png, png_block_t *block, size_t index) {
    if (png == NULL)
        return -1;

    if (block == NULL)
        return -2;

    png_node_t *at = __png_get_at(png, index);
    if (at == NULL || png_node_is_tail(at))
        return -3;

    png_node_t *node = png_node_create(block);
    if (node == NULL)
        return -4;

    node->next = at->next;
    node->prev = at;
    at->next = node;

    png->length += 1;

    png->size = PNG_BLOCK_BASE_SIZE + png_block_get_length(block);

    return 0;
}

png_node_t *__png_remove(png_t* png, png_node_t *node) {
    if (png == NULL)
        return NULL;

    if (node == NULL)
        return NULL;

    if (png_node_is_head(node) || png_node_is_tail(node))
        return NULL;

    png_node_t *prev = node->prev;
    png_node_t *next = node->next;

    prev->next = next;
    next->prev = prev;

    node->next = NULL;
    node->prev = NULL;

    png->length -= 1;

    png->size -= (PNG_BLOCK_BASE_SIZE + png_block_get_length(node->block));

    return node;
}

png_node_t *__png_find(png_t *png, png_block_t *block) {
    if (png == NULL)
        return NULL;

    if (block == NULL)
        return NULL;

    for (png_node_t* current = png->head; current; current = current->next)
        if (png_block_compare(block, current->block) == 0)
            return current;

    return NULL;
}

png_block_t *png_remove(png_t *png, png_block_t *block) {
    png_node_t *node = __png_remove(png, __png_find(png, block));
    if (node == NULL)
        return NULL;
    png_node_destroy(node);
    return block;
}

png_block_t *png_remove_at(png_t *png, size_t index) {
    png_node_t *node = __png_get_at(png, index);
    if (node == NULL)
        return NULL;
    png_block_t *block = node->block;
    png_node_destroy(node);
    return block;
}
