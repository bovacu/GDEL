#include "Vm/include/memBlock.h"
#include "Vm/include/memory.h"
#include "Vm/include/data.h"

/*
 *  To init a block we need to pass it as parameter first, we could of course use a more C++ way and just use new, but as for
 *  freeing up memory, in the method we take a void*, delete void* causes undefined behaivour and we have to use free() from C,
 *  so to, at least, keep a coerence with memory part, we will use the standard C way.
 *  
 *  The method just adds the basic data to the memBlock.
*/
void initGdelMemBlock(gdelMemBlock* _memBlock) {
    _memBlock->byteCode = nullptr;
    _memBlock->currentByteIndex = 0;
    _memBlock->maxByteSize = 0;
    initGdelDataPool(&_memBlock->dataPool);
}


/*
 *  This methods has two parts:
 *      - There's space: if theres space available inside the dynamic array of bytes, we simply add a new one at the end
 *        and update the memBlock index to the position of this last byte.
 *      - There's no more space: First we increase the capacity of our memBlock (size_t maxByteSize) and the, by reallocating
 *        increase the dynamic array size to the maxByteSize.
*/
void writeToGdelMemBlock(gdelMemBlock* _memBlock, byte _byte) {
    if(_memBlock->maxByteSize <= _memBlock->currentByteIndex) {
        size_t _currentByteCodeSize = _memBlock->maxByteSize;
        _memBlock->maxByteSize = INCREASE_GDEL_MEM_BLOCK_CAPACITY(_currentByteCodeSize);
        _memBlock->byteCode = INCREASE_GDEL_MEM_BLOCK_BYTES(uint8_t, _memBlock->byteCode, _currentByteCodeSize, _memBlock->maxByteSize);
    }

    _memBlock->byteCode[_memBlock->currentByteIndex++] = _byte;
}

/*
 * To write a constant, we have to write it to the data pool dynamic array of the memBlock, this way it is stored in that memory
 * stack and its accessible from the byteCode from the memBlock.
 * 
 * Then we return the index (that's why the -1, is the index in the array) where the data was stored inside the DataPool to be able
 * to locate it later.
*/
size_t writeConstantToGdelMemBlock(gdelMemBlock* _memBlock, gdelData _data) {
    writeToDataPool(&_memBlock->dataPool, _data);
    return _memBlock->dataPool.currentDataValue - 1;
}


/*
 * Once we don't need a memBlock, we need to dispose it to free the memory, to do so we call realloc with a size of 0, which for 
 * our VM means to free the memory. It also frees the DataPool associated to the MemBlock.
*/
void freeGdleMemBlock(gdelMemBlock* _memBlock) {
    FREE_GDEL_MEM_BLOCK(uint8_t, _memBlock->byteCode, _memBlock->maxByteSize);
    freeDataPool(&_memBlock->dataPool);
    initGdelMemBlock(_memBlock);
}
