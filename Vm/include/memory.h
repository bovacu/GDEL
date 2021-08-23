#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#define MINIMUM_GDEL_MEM_BLOCK_SIZE 8
#define GDEL_MEM_BLOCK_SCALE_FACTOR 2
#define INCREASE_GDEL_MEM_BLOCK_CAPACITY(size) ((size) < MINIMUM_GDEL_MEM_BLOCK_SIZE ? MINIMUM_GDEL_MEM_BLOCK_SIZE : (size) * GDEL_MEM_BLOCK_SCALE_FACTOR)
#define INCREASE_GDEL_MEM_BLOCK_BYTES(byteType, byteCodeArrPointer, oldSize, newSize) (byteType*)reallocate(byteCodeArrPointer, sizeof(byteType) * (oldSize), \
                                                                          sizeof(byteType) * (newSize))
#define FREE_GDEL_MEM_BLOCK(byteType, byteCodeArrPointer, oldSize) ((byteType*)reallocate(byteCodeArrPointer, sizeof(byteType) * (oldSize), 0))

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // __MEMORY_H__
