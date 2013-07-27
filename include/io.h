

#ifndef __LIBPNGPACK_TYPE_H__
#define __LIBPNGPACK_TYPE_H__ 1


#include "types.h"


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


char* memblk_to_s(memblk_t* block);
int memblk_fwrite(memblk_t* block, FILE* fp);
//int memblk_fread(memblk_t* block, size_t size, FILE* fp);

char* png_block_to_s(png_block_t* block);
int png_block_fwrite(png_block_t* block, FILE* fp);
//int png_block_fread(png_block_t* block, FILE* fp);


#ifdef __cplusplus
};
#endif

#endif