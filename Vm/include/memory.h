#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#define GDEL_ALLOCATE_HEAP(_type, _count) (_type*)reallocate(nullptr, 0, sizeof(_type) * (_count))
#define GDEL_FREE_HEAP(_type, _ptr) reallocate(_ptr, sizeof(_type), 0)

#define MINIMUM_GDEL_MEM_BLOCK_SIZE 8
#define GDEL_MEM_BLOCK_SCALE_FACTOR 2
#define INCREASE_GDEL_MEM_BLOCK_CAPACITY(size) ((size) < MINIMUM_GDEL_MEM_BLOCK_SIZE ? MINIMUM_GDEL_MEM_BLOCK_SIZE : (size) * GDEL_MEM_BLOCK_SCALE_FACTOR)
#define INCREASE_GDEL_MEM_BLOCK_BYTES(byteType, byteCodeArrPointer, oldSize, newSize) (byteType*)reallocate(byteCodeArrPointer, sizeof(byteType) * (oldSize), \
                                                                          sizeof(byteType) * (newSize))
#define GDEL_FREE_BLOCK(byteType, byteCodeArrPointer, oldSize) ((byteType*)reallocate(byteCodeArrPointer, sizeof(byteType) * (oldSize), 0))


#endif // __MEMORY_H__
