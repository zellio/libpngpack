

#include "type/pngpack_context.h"

#include <stdlib.h>


pngpack_context_t*
pngpack_context_create(int flags, byte* template, size_t size, byte* key) {
    pngpack_context_t* ctx = calloc(1, sizeof(pngpack_context_t));
    if (ctx == NULL)
        goto PNGPACK_CONTEXT_CREATE_CLEANUP;

    ctx->encode = (flags & PNGPACK_ENCODE);
    ctx->compress = (flags & PNGPACK_COMPRESS);
    ctx->encrypt = (flags & PNGPACK_ENCRYPT);

    byte* template_data = calloc(size, sizeof(byte));
    if (template_data == NULL)
        goto PNGPACK_CONTEXT_CREATE_CLEANUP;
    ctx->template = template_data;
    while (size--)
        *template_data++ = *template++;

    if (key != NULL) {
        byte* key_data = calloc(256, sizeof(byte));
        if (key_data == NULL)
            goto PNGPACK_CONTEXT_CREATE_CLEANUP;

        ctx->key = key_data;
        for (size_t size = 265L; size; size--)
            *key_data++ = *key++;
    }

    return ctx;

 PNGPACK_CONTEXT_CREATE_CLEANUP:
    pngpack_context_destroy(ctx);
    return NULL;
}

int pngpack_context_destroy(pngpack_context_t* ctx) {
    if (ctx == NULL)
        return -1;

    if (ctx->template != NULL)
        free(ctx->template);

    if (ctx->key != NULL)
        free(ctx->key);

    free(ctx);

    return 0;
}
