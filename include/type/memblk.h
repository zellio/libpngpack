

#ifndef __LIBPNGPACK_TYPE_MEMBLK_H__
#define __LIBPNGPACK_TYPE_MEMBLK_H__ 1


#include "type/byte.h"

#include <openssl/evp.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


#define MEMBLK_SEEK_SET 1
#define MEMBLK_SEEK_CUR 2
#define MEMBLK_SEEK_END 3



typedef struct memblk memblk_t;
struct memblk {
    size_t size;
    byte *data;
    byte *ptr;
    byte *end;
};


memblk_t *memblk_create(size_t size);
int memblk_destroy(memblk_t *block);

ssize_t memblk_write(memblk_t *block, byte *data, size_t count);
ssize_t memblk_read(memblk_t *block, byte *data, size_t count);
ssize_t memblk_seek(memblk_t *block, size_t offset, int whence);
ssize_t memblk_rewind(memblk_t *block);
ssize_t memblk_erase(memblk_t *block);

ssize_t memblk_write_uint32(memblk_t *block, uint32_t ui32);
ssize_t memblk_read_uint32(memblk_t *block, uint32_t *ui32_ptr);

char *memblk_x64_encode(memblk_t *block);
memblk_t *memblk_x64_decode(char *str);

ssize_t memblk_contents_x64_encode(memblk_t *block);
ssize_t memblk_contents_x64_decode(memblk_t *block);

ssize_t memblk_contents_deflate(memblk_t *block);
ssize_t memblk_contents_inflate(memblk_t *block);

ssize_t memblk_contents_encrypt(memblk_t *block, EVP_CIPHER_CTX *ctx);
ssize_t memblk_contents_decrypt(memblk_t *block, EVP_CIPHER_CTX *ctx);


#ifdef __cplusplus
};
#endif

#endif
