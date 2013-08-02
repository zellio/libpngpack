

#ifndef __LIBPNGPACK_TYPE_PNG_H__
#define __LIBPNGPACK_TYPE_PNG_H__ 1


#include "type/byte.h"
#include "type/memblk.h"
#include "type/png_block.h"
#include "type/pngpack_context.h"

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


#define PNG_BASE_SIZE 20


typedef struct png_node png_node_t;
struct png_node {
    struct png_node* prev;
    struct png_node* next;
    png_block_t* block;
};

typedef struct png png_t;
struct png {
    png_node_t* head;
    png_node_t* tail;
    size_t length;
    size_t size;
};

png_node_t* png_node_create(png_block_t* block);
int png_node_destroy(png_node_t* node);

png_t* png_create(void);
int png_destroy(png_t* png);
int png_clear(png_t* png);
png_node_t* png_get_at(png_t* png, size_t index);
int png_index_of(png_t* png, png_node_t* node);
int png_contains(png_t* png, png_node_t* node);
int png_add(png_t* png, png_node_t* node);
int png_add_at(png_t* png, png_node_t* node, size_t index);
png_node_t* png_remove(png_t* png, png_node_t* node);
png_node_t* png_remove_at(png_t* png, size_t index);



#ifdef __cplusplus
};
#endif

#endif
