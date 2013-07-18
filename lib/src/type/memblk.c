

#include "type/memblk.h"

#include <openssl/aes.h>
#include <stdlib.h>
#include <zlib.h>


memblk_t* memblk_create(size_t size) {
    memblk_t* block = calloc(1, sizeof(memblk_t));
    if (block == NULL)
        return NULL;

    block->size = size;
    block->data = calloc(size, sizeof(byte));
    block->ptr = block->data;
    block->end = block->ptr + block->size;

    if (block->data == NULL) {
        free(block);
        return NULL;
    }

    return block;
}

int memblk_destroy(memblk_t* block) {
    if (block == NULL)
        return -1;

    if (block->data == NULL) {
        free(block);
        return -2;
    }

    free(block->data);
    free(block);
    return 0;
}

ssize_t memblk_read(memblk_t* block, byte* data, size_t count) {
    byte* block_ptr = block->ptr;
    byte* block_end = block->end;

    ssize_t write_count = 0;
    while (block_end != block_ptr && count--) {
        *data++ = *block_ptr++;
        write_count++;
    }

    block->ptr = block_ptr;
    return write_count;
}

ssize_t memblk_write(memblk_t* block, byte* data, size_t count) {
    byte* block_ptr = block->ptr;
    byte* block_end = block->end;

    ssize_t write_count = 0;
    while (block_end != block_ptr && count--) {
        *block_ptr++ = *data++;
        write_count++;
    }

    block->ptr = block_ptr;
    return write_count;
}

ssize_t memblk_seek(memblk_t* block, size_t offset, int whence) {
    byte* start;
    switch (whence) {
    case MEMBLK_SEEK_SET:  start = block->data;  break;
    case MEMBLK_SEEK_CUR:  start = block->ptr;   break;
    case MEMBLK_SEEK_END:  start = block->end;   break;
    default: return -1;
    }

    byte* target = start + offset;
    byte* end = block->end;
    if (target > end)
        return -2;

    block->ptr = target;
    return 0;
}

ssize_t memblk_rewind(memblk_t* block) {
    return memblk_seek(block, 0, MEMBLK_SEEK_SET);
}

ssize_t memblk_write_uint32(memblk_t* block, uint32_t ui32) {
    byte* ptr = block->ptr;
    if (ptr + 4 > block->end)
        return -1;

    *ptr++ = (ui32 >> 24) & 0xff;
    *ptr++ = (ui32 >> 16) & 0xff;
    *ptr++ = (ui32 >> 8) & 0xff;
    *ptr++ = (ui32) & 0xff;

    block->ptr = ptr;

    return 4;
}

ssize_t memblk_read_uint32(memblk_t* block, uint32_t* ui32_ptr) {
    byte* ptr = block->ptr;
    if (ptr + 4 > block->end)
        return -1;

    *ui32_ptr += ((*ptr++) << 24);
    *ui32_ptr += ((*ptr++) << 16);
    *ui32_ptr += ((*ptr++) << 8);
    *ui32_ptr += ((*ptr++) << 0);

    return 4;
}

static char x64_digest_encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H','I', 'J', 'K', 'L', 'M', 'N', 'O', 'P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X','Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f','g', 'h', 'i', 'j', 'k', 'l', 'm', 'n','o', 'p', 'q', 'r', 's', 't', 'u', 'v','w', 'x', 'y', 'z', '0', '1', '2', '3','4', '5', '6', '7', '8', '9', '+', '/'};

char* memblk_x64_encode(memblk_t* block) {
    size_t size = block->size;
    size_t target_size = (size + 2) / 3 * 4;

    char* out = calloc(1, target_size);
    char* ptr = out;

    if (out == NULL)
        return NULL;

    size_t i = 0;
    while (i < size) {
        uint32_t o1 = i < size ? block->data[i++] : 0,
                 o2 = i < size ? block->data[i++] : 0,
                 o3 = i < size ? block->data[i++] : 0,
                 triple = (o1 << 0x10) + (o2 << 0x08) + o3;

        for (int k = 3; k >= 0; k--)
            *ptr++ = x64_digest_encoding_table[(triple >> k * 6) & 0x3F];
    }

    switch (size % 3) {
    case 1: *--ptr = '=';
    case 2: *--ptr = '='; break;
    }

    return out;
}

#define X64_TO_I(x) ((('A'<=(x))&&((x)<='Z'))?((x)-'A'):          \
                     ((('a'<=(x))&&((x)<='z'))?((x)-'a'+26):      \
                      ((('0'<=(x))&&((x)<='9'))?((x)-'0'+52):     \
                       (((x)=='+')?(62):(((x)=='/')?(63):(0))))))

memblk_t* memblk_x64_decode(char* x64str) {
    size_t str_len = 0;
    char* ptr = x64str;
    while (*ptr++) str_len++;

    size_t size = str_len * 3 / 4;

    if (x64str[str_len - 1] == '=') size--;
    if (x64str[str_len - 2] == '=') size--;

    memblk_t* memblk = memblk_create(size);
    byte* data = memblk->data;

    size_t i = 0;
    while (i < size) {
        uint32_t s1 = X64_TO_I(*x64str); x64str++;
        uint32_t s2 = X64_TO_I(*x64str); x64str++;
        uint32_t s3 = X64_TO_I(*x64str); x64str++;
        uint32_t s4 = X64_TO_I(*x64str); x64str++;
        uint32_t t = (s1 << 18) + (s2 << 12) + (s3 << 6) + s4;

        if (i < size) data[i++] = (t >> 16) & 0xFF;
        if (i < size) data[i++] = (t >> 8) & 0xFF;
        if (i < size) data[i++] = t & 0xFF;
    }

    return memblk;
}

ssize_t memblk_contents_x64_encode(memblk_t* block) {
    size_t size = block->size;
    size_t target_size = (size + 2) / 3 * 4;
    char* target_data = memblk_x64_encode(block);
    block->data = (byte*)target_data;
    block->size = target_size;
    return target_size;
}

ssize_t memblk_contents_x64_decode(memblk_t* block) {
    char* str = calloc(block->size + 1, sizeof(byte));
    if (str == NULL) return -1;
    snprintf(str, block->size + 1, "%s", block->data);
    memblk_t* new_block = memblk_x64_decode(str);
    free(str);
    free(block->data);
    block->size = new_block->size;
    block->data = new_block->data;
    free(new_block);
    return block->size;
}

ssize_t memblk_contents_deflate(memblk_t* block) {
    byte* data = block->data;
    size_t size = block->size;
    size_t compressed_size = compressBound(size) + 4;
    byte* buffer = calloc(compressed_size, sizeof(byte));

    if (buffer == NULL)
        return -1;

    compress2(buffer + 4, &compressed_size, data, size, 9);

    size_t new_block_size = compressed_size + 4;

    block->data = realloc(buffer, new_block_size);
    block->size = new_block_size;

    memblk_rewind(block);
    memblk_write_uint32(block, size);

    free(data);

    return compressed_size;
}

ssize_t memblk_contents_inflate(memblk_t* block) {
    byte* block_data = block->data;
    size_t size = block->size;

    size_t decompressed_size;
    memblk_rewind(block);
    memblk_read_uint32(block, (uint32_t*)&decompressed_size);
    byte* data = block->ptr;

    byte* decompressed_data = calloc(decompressed_size, sizeof(byte));
    if (decompressed_data == NULL)
        return -1;

    uncompress(decompressed_data, &decompressed_size, data, size);

    block->data = decompressed_data;
    block->size = decompressed_size;

    free(block_data);

    return decompressed_size;
}

ssize_t memblk_contents_encrypt(memblk_t* block, EVP_CIPHER_CTX* ctx) {
    byte* in_data = block->data;
    int32_t in_size = block->size;

    int32_t out_size = in_size + AES_BLOCK_SIZE - 1;
    byte* out_data = calloc(out_size, sizeof(byte));

    if (out_data == NULL)
        return -1;

    int32_t final_size;
    EVP_EncryptInit_ex(ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(ctx, out_data, &out_size, in_data, in_size);
    EVP_EncryptFinal_ex(ctx, out_data + out_size, &final_size);

    free(block->data);

    block->data = out_data;
    block->size = out_size + final_size;

    return out_size;
}

ssize_t memblk_contents_decrypt(memblk_t* block, EVP_CIPHER_CTX* ctx) {
    byte* in_data = block->data;
    int32_t in_size = block->size;

    int32_t out_size = in_size + AES_BLOCK_SIZE - 1;
    byte* out_data = calloc(out_size, sizeof(byte));

    if (out_data == NULL)
        return -1;

    int32_t final_size;
    EVP_DecryptInit_ex(ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(ctx, out_data, &out_size, in_data, in_size);
    EVP_DecryptFinal_ex(ctx, out_data + out_size, &final_size);

    free(block->data);

    block->data = out_data;
    block->size = out_size + final_size;

    return out_size;
}
