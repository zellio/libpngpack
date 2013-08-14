

#ifndef __LIBPNGPACK_TYPE_CONTEXT_H__
#define __LIBPNGPACK_TYPE_CONTEXT_H__ 1


#include "byte.h"

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


#define PNGPACK_ENCODE 1
#define PNGPACK_COMPRESS 2
#define PNGPACK_ENCRYPT 4


typedef struct pngpack_context pngpack_context_t;
struct pngpack_context {
    byte encode;
    byte compress;
    byte encrypt;
    byte *template;
    byte *key;
};

pngpack_context_t*
pngpack_context_create(int flags, byte *template, size_t size, byte *key);
int pngpack_context_destroy(pngpack_context_t *ctx);


#ifdef __cplusplus
};
#endif

#endif
