

#include "type/png.h"

#include <stdlib.h>


png_node_t* png_node_create(png_block_t* block) {
    png_node_t* node = calloc(1, sizeof(png_node_t));
    if (node == NULL)
        return NULL;

    node->block = block;

    return node;
}

int png_node_destroy(png_node_t* node) {
    if (node == NULL)
        return -1;
    free(node);
    return 0;
}

png_t* png_create(void) {
    png_t* png = calloc(1, sizeof(png));
    if (png == NULL)
        return 0;

    png_block_t* head_block = png_block_create_empty();
    png_block_set_type(head_block, 0x89504E47);
    png_block_set_crc(head_block, 0x0D0A1A0A);

    png_node_t* head = png_node_create(head_block);
    png_node_t* tail = png_node_create(png_block_create(0x49454e44, 0x0, NULL));

    head->prev = NULL;
    head->next = tail;

    tail->prev = head;
    tail->prev = NULL;

    png->length = 2;
    png->head = head;
    png->tail = tail;

    return png;
}

int png_destroy(png_t* png) {
    if (png == NULL)
        return -1;

    png_clear(png);
    free(png);

    return 0;
}

int png_clear(png_t* png) {
    if (png == NULL)
        return -1;

    png_node_t* current;
    png_node_t* ptr;
    png_node_t* head = png->head;
    png_node_t* tail = png->tail;

    current = head->next;

    while (current != tail) {
        ptr = current;
        current = current->next;
        png_node_destroy(ptr);
    }

    head->next = tail;
    tail->prev = head;

    png->length = 2;

    return 0;
}

png_node_t* png_get_at(png_t* png, size_t index) {
    if (png == NULL)
        return NULL;

    if (png->length <= index)
        return NULL;

    png_node_t* ptr = png->head;
    while (index-- && (ptr = ptr->next));

    return ptr;
}

int png_index_of(png_t* png, png_node_t* node) {
    if (png == NULL)
        return -1;

    if (node == NULL)
        return -1;

    int index = 0;
    for (png_node_t* current = png->head; current; current = current->next) {
        if (node == current)
            return index;
        index++;
    }

    return -1;
}

int png_contains(png_t* png, png_node_t* node) {
    return (png_index_of(png, node) != -1);
}

int png_add(png_t* png, png_node_t* node) {
    if (png == NULL)
        return -1;

    if (node == NULL)
        return -1;

    png_node_t* tail = png->tail;
    png_node_t* prev = tail->prev;

    prev->next = node;
    node->prev = prev;
    node->next = tail;
    tail->prev = node;

    png->length += 1;

    return 0;
}

int png_add_at(png_t* png, png_node_t* node, size_t index) {
    if (png == NULL)
        return -1;

    if (node == NULL)
        return -2;

    png_node_t* at = png_get_at(png, index);
    if (at == NULL || at == png->tail)
        return -3;

    node->next = at->next;
    node->prev = at;
    at->next = node;

    png->length += 1;

    return 0;
}

png_node_t* png_remove(png_t* png, png_node_t* node) {
    if (png == NULL)
        return NULL;

    if (node == NULL)
        return NULL;

    if (node == png->head || node == png->tail)
        return NULL;

    if (!(png_contains(png, node)))
        return NULL;

    png_node_t* prev = node->prev;
    png_node_t* next = node->next;

    prev->next = next;
    next->prev = prev;

    node->next = NULL;
    node->prev = NULL;

    png->length -= 1;

    return node;

}

png_node_t* png_remove_at(png_t* png, size_t index) {
    if (png == NULL)
        return NULL;

    png_node_t* at = png_get_at(png, index);
    if (at == NULL)
        return NULL;

    if (at == png->head || at == png->tail)
        return NULL;

    png_node_t* prev = at->prev;
    png_node_t* next = at->next;

    prev->next = next;
    next->prev = prev;

    at->next = NULL;
    at->prev = NULL;

    return at;
}
