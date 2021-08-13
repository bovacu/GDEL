#include "Vm/include/memory.h"

/*
 * This method takes the current array of bytes, its oldSize and new size to increase or completele free from memory.
 * If a 0 is passed, we will free the memory from this dynamic array and in other case, realloc will move the dynamic array
 * to another conrtiguous piece of memory that satisfies the newSize, copying all the current data to the new address.
*/
void* reallocate(void* _byteCodeArrPtr, size_t _oldSize, size_t _newSize) {
    if(_newSize == 0) {
        free(_byteCodeArrPtr);
        return nullptr;
    }

    void* _reallocatedByteCodeArrPtr = realloc(_byteCodeArrPtr, _newSize);
    if(_reallocatedByteCodeArrPtr == nullptr) exit(EXIT_NO_POSSIBLE_REALLOCATION_ERROR);
    return _reallocatedByteCodeArrPtr;
}