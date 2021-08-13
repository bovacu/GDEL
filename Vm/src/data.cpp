#include "Vm/include/data.h"
#include "Vm/include/memory.h"

/*
 * Those methods do basically the same as the memBlock.
*/
void initGdelDataPool(gdelDataPool* _dataPool) {
    _dataPool->pool = nullptr;
    _dataPool->maxSize = 0;
    _dataPool->currentDataValue = 0;
}

void writeToDataPool(gdelDataPool* _dataPool, gdelData _data) {
    if(_dataPool->maxSize <= _dataPool->currentDataValue) {
        size_t _currentByteCodeSize = _dataPool->maxSize;
        _dataPool->maxSize = INCREASE_GDEL_MEM_BLOCK_CAPACITY(_currentByteCodeSize);
        _dataPool->pool = INCREASE_GDEL_MEM_BLOCK_BYTES(gdelData, _dataPool->pool, _currentByteCodeSize, _dataPool->maxSize);
    }

    _dataPool->pool[_dataPool->currentDataValue++] = _data;
}

void freeDataPool(gdelDataPool* _dataPool) {
    FREE_GDEL_MEM_BLOCK(gdelData, _dataPool->pool, _dataPool->maxSize);
    initGdelDataPool(_dataPool);
}