

#ifndef __LIBPNGPACK_TYPE_PNG_NODE_H__
#define __LIBPNGPACK_TYPE_PNG_NODE_H__ 1


#include "type/png/block.h"

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


typedef struct png_node png_node_t;
struct png_node {
    struct png_node* prev;
    struct png_node* next;
    png_block_t* block;
};


png_node_t *png_node_create(png_block_t *block);
int png_node_destroy(png_node_t *node);
png_node_t *png_node_create_head();
png_node_t *png_node_create_tail();
int png_node_is_head(png_node_t *node);
int png_node_is_tail(png_node_t *node);


#ifdef __cplusplus
};
#endif

#endif
