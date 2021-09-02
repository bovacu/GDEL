#include "Vm/include/data.h"
#include "Vm/include/memory.h"
#include "Vm/include/register.h"

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
    GDEL_FREE_BLOCK(gdelData, _dataPool->pool, _dataPool->maxSize);
    initGdelDataPool(_dataPool);
}

bool areGdelDataEqual(gdelData& _left, gdelData& _right) {
    if(_left.type != _right.type) return false;

    switch(_left.type) {
        case gdelDataType::DT_BOOL:     return GET_GDEL_BOOL_DATA(_left) == GET_GDEL_BOOL_DATA(_right);
        case gdelDataType::DT_NUMBER:   return GET_GDEL_NUMBER_DATA(_left) == GET_GDEL_NUMBER_DATA(_right);
        case gdelDataType::DT_NULL:     return true;
        case gdelDataType::DT_REGISTER: return GET_GDEL_REGISTER_DATA(_left) == GET_GDEL_REGISTER_DATA(_right);
    }

    PRINT_LN_ERROR("No comparisson stablished for data type '" << _left.type << "'");
    return false;
}