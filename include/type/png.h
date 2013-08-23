

#ifndef __LIBPNGPACK_TYPE_PNG_H__
#define __LIBPNGPACK_TYPE_PNG_H__ 1


#include "type/byte.h"
#include "type/png/block.h"
#include "type/png/node.h"

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


#define PNG_BASE_SIZE 20


typedef struct png png_t;
struct png {
    png_node_t *head;
    png_node_t *tail;
    size_t length;
    size_t size;
};


png_t *png_create(void);
int png_destroy(png_t *png);
int png_clear(png_t *png);
png_block_t *png_get_at(png_t *png, size_t index);
int png_index_of(png_t *png, png_block_t *block);
int png_contains(png_t *png, png_block_t *block);
int png_add(png_t *png, png_block_t *block);
int png_add_at(png_t *png, png_block_t *block, size_t index);
png_block_t *png_remove(png_t *png, png_block_t *block);
png_block_t *png_remove_at(png_t *png, size_t index);


#ifdef __cplusplus
};
#endif

#endif
